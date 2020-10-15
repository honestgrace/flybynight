using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.NetworkInformation;
using System.Threading;
using PcapDotNet.Core;
using PcapDotNet.Packets;
using PcapDotNet.Packets.Ethernet;
using PcapDotNet.Packets.IpV4;
using PcapDotNet.Packets.Transport;

namespace TcpPlayground
{
    class Program
    {
        private const int InterfaceIndex = 1;

        private static IpV4Address SourceIp = default(IpV4Address);
        private static MacAddress SourceMac = default(MacAddress);

        //
        // app name should be "IISCrashReprod"
        //
        private static string serverIpV4Address = "10.127.66.122";
        private static string serverMac = "00-15-5D-A9-29-09";
        private static int repeatCount = 10;
        
        static IpV4Address DestIp; // = new IpV4Address(serverIpV4Address); // server IP
        static MacAddress DestMac; // = new MacAddress(serverMac.Replace('-', ':')); // server MAC (if server in same IP net) or router MAC

        static void Main(string[] args)
        {
            if (args.Length > 1)
            {
                serverIpV4Address = args[0];
                serverMac = args[1];
                repeatCount = Convert.ToInt32(args[2]);
                Console.WriteLine(serverIpV4Address + ": " + serverMac);
            }

            DestIp = new IpV4Address(serverIpV4Address); // server IP
            DestMac = new MacAddress(serverMac.Replace('-', ':')); // server MAC (if server in same IP net) or router MAC

            // Retrieve the device list from the local machine
            IList<LivePacketDevice> allDevices = LivePacketDevice.AllLocalMachine;

            if (allDevices.Count == 0)
            {
                Console.WriteLine("No interfaces found! Make sure WinPcap is installed.");
                return;
            }

            // Print the list
            for (int i = 0; i != allDevices.Count; ++i)
            {
                LivePacketDevice lpd = allDevices[i];
                Console.Write((i + 1) + ". " + lpd.Name);
                if (lpd.Description != null)
                    Console.WriteLine(" (" + lpd.Description + ")");
                else
                    Console.WriteLine(" (No description available)");
            }

            var device = allDevices[InterfaceIndex - 1];

            var ip = device.Addresses.First(a => a.Address.Family == SocketAddressFamily.Internet)
                .Address.ToString().Replace("Internet ", "");
            SourceIp = new IpV4Address(ip);
            SourceMac = new MacAddress(GetMac(device));

            var offlineDevice = new OfflinePacketDevice("clientSide.pcap");
            Random rnd = new Random((int)DateTime.Now.Ticks);

            for (int i = 0; i < repeatCount; i++)
            {
                uint seq = (uint)rnd.Next() * 1234;
                uint ack = 0;
                ushort srcPort = (ushort)(rnd.Next() & 0x7fff + 15000);

                try
                {
                    Console.WriteLine("Sending request");

                    using (var input = device.Open(65535, PacketDeviceOpenAttributes.NoCaptureLocal, 1000))
                    //using (var input = device.Open(65535, PacketDeviceOpenAttributes.Promiscuous, 1000))
                    {
                        string filterStr = $"src host {DestIp} and dst port {srcPort}";
                        input.SetFilter(filterStr);
                        using (var output = device.Open(65536, PacketDeviceOpenAttributes.Promiscuous, 1000))
                        using (PacketCommunicator pcapReader = // Open the capture file
                            offlineDevice.Open(
                                65536, // portion of the packet to capture, 65536 guarantees that the whole packet will be captured on all the link layers
                                PacketDeviceOpenAttributes.Promiscuous, // promiscuous mode
                                1000)) // read timeout
                        {
                            // read first packet from pcap file
                            pcapReader.ReceivePacket(out var packet);

                            var clientSyn = BuildTcpPacket(packet, seq, ack, srcPort);
                            // sending client SYN
                            output.SendPacket(clientSyn);
                            // receive server SYN, ACK
                            PacketCommunicatorReceiveResult res = input.ReceivePacket(out var serverSynAck);
                            pcapReader.ReceivePacket(out var clientAck);
                            // sending client ACK
                            clientAck = BuildTcpPacket(clientAck, serverSynAck.Ethernet.IpV4.Tcp.AcknowledgmentNumber,
                                serverSynAck.Ethernet.IpV4.Tcp.NextSequenceNumber + 1, srcPort);
                            output.SendPacket(clientAck);

                            // connection established. sending data
                            res = pcapReader.ReceivePacket(out var data1);
                            res = pcapReader.ReceivePacket(out var data2);
                            res = pcapReader.ReceivePacket(out var data3);
                            res = pcapReader.ReceivePacket(out var lastClientAck);

                            data1 = BuildTcpPacket(data1, serverSynAck.Ethernet.IpV4.Tcp.AcknowledgmentNumber,
                                serverSynAck.Ethernet.IpV4.Tcp.NextSequenceNumber + 1, srcPort);
                            output.SendPacket(data1);

                            res = input.ReceivePacket(out var data1Ack);
                            data2 = BuildTcpPacket(data2, data1Ack.Ethernet.IpV4.Tcp.AcknowledgmentNumber,
                                data1Ack.Ethernet.IpV4.Tcp.NextSequenceNumber, srcPort);
                            output.SendPacket(data2);

                            res = input.ReceivePacket(out var data2Ack);
                            data3 = BuildTcpPacket(data3, data2Ack.Ethernet.IpV4.Tcp.AcknowledgmentNumber,
                                data2Ack.Ethernet.IpV4.Tcp.NextSequenceNumber, srcPort);
                            output.SendPacket(data3);

                            //res = input.ReceivePacket(out var data3Ack);
                            res = input.ReceivePacket(out var response);

                            lastClientAck = BuildTcpPacket(lastClientAck,
                                response.Ethernet.IpV4.Tcp.AcknowledgmentNumber,
                                response.Ethernet.IpV4.Tcp.NextSequenceNumber + 1, srcPort);
                            output.SendPacket(lastClientAck);

                            Console.WriteLine("Request sent");
                        }
                    }
                }
                catch { }

                Thread.Sleep(1000);
            }
        }

        private static string GetMac(LivePacketDevice dev)
        {
            string devId = dev.Name.Substring(dev.Name.IndexOf('{'), dev.Name.Length - dev.Name.IndexOf('{'));
            var ifaces = NetworkInterface.GetAllNetworkInterfaces();
            var mac = ifaces.First(i => i.Id == devId).GetPhysicalAddress().ToString();

            return string.Join(":", Enumerable.Range(0, mac.Length / 2)
                .Select(i => mac.Substring(i * 2, 2)));
        }

        private static Packet BuildTcpPacket(Packet packet, uint seq, uint ack, ushort srcPort)
        {
            var ethernetLayer = new EthernetLayer
            {
                Source = SourceMac,
                Destination = DestMac,
                EtherType = EthernetType.None, // will be filled automatically
            };

            var ipv4Layer = new IpV4Layer
            {
                Source = SourceIp,
                CurrentDestination = DestIp,
                Fragmentation = packet.Ethernet.IpV4.Fragmentation,
                HeaderChecksum = null, // will be filled automatically
                Identification = packet.Ethernet.IpV4.Identification,
                Options = packet.Ethernet.IpV4.Options,
                Protocol = null, // will be filled automatically
                Ttl = packet.Ethernet.IpV4.Ttl,
                TypeOfService = packet.Ethernet.IpV4.TypeOfService
            };

            var tcpLayer = new TcpLayer //packet.Ethernet.IpV4.Tcp.ExtractLayer();
            {
                SourcePort = srcPort, //packet.Ethernet.IpV4.Tcp.SourcePort,
                DestinationPort = packet.Ethernet.IpV4.Tcp.DestinationPort,
                Checksum = null, // will be filled automatically
                SequenceNumber = seq, //packet.Ethernet.IpV4.Tcp.SequenceNumber,
                AcknowledgmentNumber = ack, //packet.Ethernet.IpV4.Tcp.AcknowledgmentNumber,
                ControlBits = packet.Ethernet.IpV4.Tcp.ControlBits,
                Window = packet.Ethernet.IpV4.Tcp.Window,
                UrgentPointer = packet.Ethernet.IpV4.Tcp.UrgentPointer,
                Options = packet.Ethernet.IpV4.Tcp.Options
            };

            var payloadLayer = new PayloadLayer
            {
                Data = packet.Ethernet.IpV4.Tcp.Payload
            };

            var builder = new PacketBuilder(ethernetLayer, ipv4Layer, tcpLayer, payloadLayer);

            return builder.Build(packet.Timestamp);
        }
    }
}
