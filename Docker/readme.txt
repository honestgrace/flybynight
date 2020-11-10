docker build . -t test
docker run --env DOCKERHOST=%computername% -v c:\docker:c:\docker --rm -it -p 80:80 -p 443:443 --entrypoint powershell.exe test