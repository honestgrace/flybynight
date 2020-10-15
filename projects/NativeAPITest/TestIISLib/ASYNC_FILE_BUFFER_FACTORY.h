#include <windows.h>

//
// declare PUT_VERB_HANDLER
//
class PUT_VERB_HANDLER;

#define ASYNC_FILE_BUFFER_SIZE_DEFAULT  1024
#define MAX_ASYNC_FILE_BUFFER_COUNT     32

class IAsyncBuffer
{
public:

	virtual ~IAsyncBuffer() {}

	virtual
		PVOID
		GetBuffer() const = 0;

	virtual
		ULONG
		GetBufferLength() const = 0;

	virtual
		ULONG
		GetValidLength() const = 0;

	virtual
		VOID
		SetValidLength(ULONG NewValidLength) = 0;
};

class IAsyncBufferFactory
{
public:

	virtual
		IAsyncBuffer*
		Alloc() = 0;

	virtual
		VOID
		Free(
			IAsyncBuffer* Buffer
		)
	{
		delete Buffer;
	}
};

class IAsyncEventSink
{
public:

	virtual
	REQUEST_NOTIFICATION_STATUS
	OnReadData(
			IAsyncBuffer* Buffer
		) 
	{
		throw "Not implemented";
		return REQUEST_NOTIFICATION_STATUS::RQ_NOTIFICATION_FINISH_REQUEST;
	};

	virtual
	REQUEST_NOTIFICATION_STATUS
	OnReadError(
			HRESULT ErrorCode
		) 
	{
		throw "Not implemented";
		return REQUEST_NOTIFICATION_STATUS::RQ_NOTIFICATION_FINISH_REQUEST;
	};

	virtual
	REQUEST_NOTIFICATION_STATUS
	OnReadEOF() 
	{
		throw "Not implemented";
		return REQUEST_NOTIFICATION_STATUS::RQ_NOTIFICATION_FINISH_REQUEST;
	};
};

class ASYNC_FILE_BUFFER : public IAsyncBuffer
{
public:

	ASYNC_FILE_BUFFER(
		DWORD bufferSize
	)
	{
		_ValidLength = 0;
		_AllocSuccess = _RawBuffer.Resize(bufferSize);
		//ZeroMemory(&_Overlapped, sizeof(OVERLAPPED));
	}

	//
	// IAsyncBuffer methods.
	//

	virtual
	PVOID
	GetBuffer() const
	{
		return _AllocSuccess ? _RawBuffer.QueryPtr() : NULL;
	}

	virtual
	ULONG
	GetBufferLength() const
	{
		return _AllocSuccess ? _RawBuffer.QuerySize() : 0;
	}

	virtual
	ULONG
	GetValidLength() const
	{
		return _ValidLength;
	}

	virtual
	VOID
	SetValidLength(
			ULONG NewValidLength
		)
	{
		DBG_ASSERT(NewValidLength <= _RawBuffer.QuerySize());
		_ValidLength = NewValidLength;
	}

	//
	// ASYNC_FILE_BUFFER methods.
	//
	OVERLAPPED*
		GetOverlapped()
	{
		return &_Overlapped;
	}

	static
	ASYNC_FILE_BUFFER*
	OverlappedToObject(
			OVERLAPPED* Overlapped
		)
	{
		return (Overlapped == NULL)
			? NULL
			: CONTAINING_RECORD(Overlapped, ASYNC_FILE_BUFFER, _Overlapped);
	}

private:

	OVERLAPPED _Overlapped;
	ULONG _ValidLength;
	BUFFER _RawBuffer;
	BOOL _AllocSuccess;
};

class ASYNC_FILE_BUFFER_FACTORY : public IAsyncBufferFactory
{
public:

	ASYNC_FILE_BUFFER_FACTORY()
	{
	}

	//
	// IAsyncBufferFactory methods.
	//

	virtual
		IAsyncBuffer*
		Alloc()
	{
		return new ASYNC_FILE_BUFFER(_BufferSize);
	}

	VOID
	SetBufferSize(
			DWORD BufferSize
		)
	{
		_BufferSize = BufferSize;
	}

	VOID
		SetHandler(
			PUT_VERB_HANDLER* NewHandler
		)
	{
		_Handler = NewHandler;
	}

private:
	PUT_VERB_HANDLER* _Handler;
	DWORD _BufferSize = ASYNC_FILE_BUFFER_SIZE_DEFAULT;
};

class CAsyncReader
{
private:
public:
	CAsyncReader()
	{
		Init(NULL, NULL, Unlimited, Unlimited);
	}

	~CAsyncReader()
	{
		DBG_ASSERT(_PendingBufferCount == 0);
		DBG_ASSERT(_PendingBufferLength == 0);
		DBG_ASSERT(_PendingReadBuffer == NULL);
	}

	VOID
		Init(
			IAsyncBufferFactory * BufferFactory,
			IAsyncEventSink * EventSink,
			ULONG MaxBufferCount,
			ULONG MaxBufferLength
		)
	{
		_BufferFactory = BufferFactory;
		_PendingReadBuffer = NULL;
		_EventSink = EventSink;

		_MaxBufferCount = MaxBufferCount;
		_MaxBufferLength = MaxBufferLength;
		_PendingBufferCount = 0;
		_PendingBufferLength = 0;

		_Done = FALSE;
		_EOF = FALSE;
	}

	REQUEST_NOTIFICATION_STATUS
	StartRead() {
		HRESULT hr = S_OK;
		IAsyncBuffer* buffer = NULL;
		DWORD bytesReceived = 0;
		BOOL completionPending = FALSE;
		REQUEST_NOTIFICATION_STATUS status = RQ_NOTIFICATION_PENDING;

		if (_PendingReadBuffer)
		{
			buffer = _PendingReadBuffer;
			_PendingReadBuffer = NULL;
		}

		while (TRUE)
		{
			if (buffer == NULL)
			{
				//
				// Allocate a new, fresh buffer and initiate the read.
				//

				buffer = _BufferFactory->Alloc();
			}

			if (buffer == NULL || buffer->GetBuffer() == NULL)
			{
				hr = HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);
			}
			else
			{
				//
				// _PendingReadBuffer exists for the sole benefit of the
				// IoCompleteRequest() method. It contains a pointer to
				// the pending read buffer. There Can Be Only One (tm).
				//
				// If ReadEntityBody() return S_OK with the CompletionPending
				// flag set then the read will complete asynchronously. It is
				// entirely possible for the completion routine to fire before
				// ReadEntityBody() has returned to us. Because of this, we
				// must carefully setup our state before initiating the read.
				//
				// We'll set the _PendingReadBuffer member to the current
				// before *before* initating the read. If ReadEntityBody()
				// completes synchronously, then we'll NULL it out and get on
				// with our lives. Otherwise, the read buffer will be handled
				// in IoCompleteRequest().
				//

				_PendingReadBuffer = buffer;
				completionPending = FALSE;

				DBG_ASSERT(buffer->GetBufferLength() >= buffer->GetValidLength());
				hr = ReadEntityBody(
					reinterpret_cast<BYTE*>(buffer->GetBuffer()) + buffer->GetValidLength(),    // buffer offset
					buffer->GetBufferLength() - buffer->GetValidLength(),                       // remaining buffer length
					TRUE,
					&bytesReceived,
					&completionPending
				);

				if (FAILED(hr) || !completionPending)
				{
					//
					// There will be no async I/O completion, so undo the
					// buffer set we did before calling ReadEntityBody().
					//

					_PendingReadBuffer = NULL;
				}
			}

			if (hr == HRESULT_FROM_WIN32(ERROR_HANDLE_EOF))
			{
				//
				// Map this EOF indication to a successful zero-byte read.
				//

				hr = S_OK;
				bytesReceived = 0;
				completionPending = FALSE;
			}

			//
			// If the read failed in-line, tell the client and bail.
			//

			if (FAILED(hr))
			{
				_BufferFactory->Free(buffer);
				buffer = NULL;
				_Done = TRUE;
				return _EventSink->OnReadError(hr);
			}

			//
			// If the read will complete asynchronously, then bail. The
			// _PendingReadBuffer will be freed in IoCompleteRequest().
			//

			if (completionPending)
			{
				return RQ_NOTIFICATION_PENDING;
			}

			//
			// At this point, we know the read has completed successfully
			// in-line. If we've hit EOF, then notify the client and bail.
			//

			if (bytesReceived == 0)
			{
				//
				// We've hit EOF here.
				// Call _EventSink>OnReadEOF() after flushing the read buffer.
				//

				_Done = TRUE;
				_EOF = TRUE;
			}

			//
     		// Reset the buffer valid length with adding the received bytes.
			//

			buffer->SetValidLength(buffer->GetValidLength() + bytesReceived);

			if (buffer->GetBufferLength() - buffer->GetValidLength() != 0 && !_Done)
			{
				//
				// Buffer is not filled yet. Unless we hit the end of file,
				// don't call OnReadData() and continue to read the buffer
				// until buffer gets filled.
				//

				continue;
			}

			//
			// We have actual, real entity body data and buffer is either full or we have reached EOF. Notify the client.
			//

			status = _EventSink->OnReadData(buffer);

			if (status == RQ_NOTIFICATION_PENDING)
			{
				//
				// The client now owns the buffer and is responsible for
				// releasing it "later".
				//
				// CODEWORK: This will require synchronization when we
				// implement overlapped file writes for PUT.
				//

				_PendingBufferCount++;
				_PendingBufferLength += buffer->GetValidLength();
				buffer = NULL;

				if (!IsWithinQuotaLimits())
				{
					break;
				}
			}
			else
			{
				//
				// The client is done with the buffer.
				//

				_BufferFactory->Free(buffer);
				buffer = NULL;

				if (_EOF)
				{
					return _EventSink->OnReadEOF();
				}

				if (status == RQ_NOTIFICATION_FINISH_REQUEST)
				{
					_Done = TRUE;
					break;
				}
			}
		}
		return status;
	}


	//
	// Called by the module upon async ReadEntityBody() completion.
	//

	REQUEST_NOTIFICATION_STATUS
		OnIoComplete(
			MyCompleletionInfo * CompletionInfo
		)
	{
		HRESULT hr;
		IAsyncBuffer* buffer;
		ULONG bytesReceived;
		REQUEST_NOTIFICATION_STATUS status = RQ_NOTIFICATION_PENDING;

		//
		// Snag the one-and-only pending buffer.
		//

		buffer = _PendingReadBuffer;

		DBG_ASSERT(buffer != NULL);

		hr = CompletionInfo->GetCompletionStatus();
		bytesReceived = CompletionInfo->GetCompletionBytes();

		if (hr == HRESULT_FROM_WIN32(ERROR_HANDLE_EOF))
		{
			//
			// Map this EOF indication to a successful zero-byte read.
			//

			hr = S_OK;
			bytesReceived = 0;
		}

		if (FAILED(hr))
		{
			_BufferFactory->Free(buffer);
			buffer = NULL;
			_Done = TRUE;
			return _EventSink->OnReadError(hr);
		}

		//
		// If we've hit EOF, then notify the client and bail.
		//

		if (bytesReceived == 0)
		{
			//
			// We've hit EOF here.
			// Call _EventSink>OnReadEOF() after flushing the read buffer.
			//

			_Done = TRUE;
			_EOF = TRUE;
		}

		//
		// Reset the buffer valid length with adding the received bytes.
		//

		buffer->SetValidLength(buffer->GetValidLength() + bytesReceived);

		if (buffer->GetBufferLength() - buffer->GetValidLength() == 0 || _Done)
		{
			//
			// We have actual, real entity body data and buffer is either full or we have reached EOF. Notify the client.
			//

			_PendingReadBuffer = NULL;

			status = _EventSink->OnReadData(buffer);

			if (status == RQ_NOTIFICATION_PENDING)
			{
				//
				// The client now owns the buffer and is responsible for
				// releasing it "later".
				//
				// CODEWORK: This will require synchronization when we
				// implement overlapped file writes for PUT.
				//

				_PendingBufferCount++;
				_PendingBufferLength += buffer->GetValidLength();
				buffer = NULL;
			}
			else
			{
				//
				// The client is done with the buffer.
				//

				_BufferFactory->Free(buffer);
				buffer = NULL;

				if (_EOF)
				{
					return _EventSink->OnReadEOF();
				}
			}
		}

		if (status == RQ_NOTIFICATION_FINISH_REQUEST)
		{
			_Done = TRUE;
		}
		else
		{
			//
			// If we're still below the buffer quota then initiate a new read.
			//
			// N.B. StartRead() may call ReadEntityBody() with may complete
			// asynchronously -- in fact, it may complete in another thread
			// before StartRead() returns to us. It is therefore important
			// to NOT touch any object member variables after calling StartRead().
			//

			if (IsWithinQuotaLimits())
			{
				status = StartRead();
			}
		}

		return status;
	}

	REQUEST_NOTIFICATION_STATUS
	ReleaseBuffer(
			IAsyncBuffer* Buffer
		)
	{
		//
		// Update our statistics, then free the buffer.
		//
		// CODEWORK: This will require synchronization when we
		// implement overlapped file writes for PUT.
		//

		_PendingBufferCount--;
		_PendingBufferLength -= Buffer->GetValidLength();

		_BufferFactory->Free(Buffer);

		//
		// If we're now below the buffer quota (and if we're still
		// interested in more data) then initiate a new read.
		//

		//if (!_Done && IsWithinQuotaLimits())
		if (IsWithinQuotaLimits())
		{
			return StartRead();
		}

		return RQ_NOTIFICATION_CONTINUE;
	}

private:
	BOOL
	IsWithinQuotaLimits() const
	{
		return ((_MaxBufferCount == Unlimited || _PendingBufferCount < _MaxBufferCount) &&
			(_MaxBufferLength == Unlimited || _PendingBufferLength < _MaxBufferLength));
	}

	const ULONG Unlimited = (ULONG)-1L;

	IAsyncBufferFactory * _BufferFactory;
	IAsyncBuffer * _PendingReadBuffer;
	IAsyncEventSink * _EventSink;

	ULONG _MaxBufferCount;
	ULONG _MaxBufferLength;
	ULONG _PendingBufferCount;
	ULONG _PendingBufferLength;

	BOOL _Done;
	BOOL _EOF;
};

class PUT_VERB_HANDLER : public IAsyncEventSink {

public:
	PUT_VERB_HANDLER() {
		_BufferFactory.SetHandler(this);
		_FileHandle = INVALID_HANDLE_VALUE;
		_FileLength = 0;

		_BufferFactory.SetBufferSize(READBUFFERSIZE);
	}

	~PUT_VERB_HANDLER() {
		if (_FileHandle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(_FileHandle);
			_FileHandle = INVALID_HANDLE_VALUE;
		}
	}

	REQUEST_NOTIFICATION_STATUS
	Execute()
	{
		HRESULT hr = DoCreateFile();

		if (SUCCEEDED(hr))
		{
			return StartRead(
				&_BufferFactory,
				MAX_ASYNC_FILE_BUFFER_COUNT,
				//
				// The range validation of readBufferSize in schema ensures that the below multiplication will not cause ulong overflow.
				// The maximum allowed readBufferSize is (ULONG_MAX / MAX_ASYNC_FILE_BUFFER_COUNT).
				//
				READBUFFERSIZE* MAX_ASYNC_FILE_BUFFER_COUNT
			);
		}

		return RQ_NOTIFICATION_FINISH_REQUEST;
	}

	HRESULT
	DoCreateFile() {
		//  Creates the new output file
		wstring outputFile = L"\\\\jhkim-dev2\\share\\data\\test" + to_wstring(GetTickCount64()) + L".txt";
		//wstring outputFile = L"c:\\temp\\output\\test" + to_wstring(GetTickCount64()) + L".txt";
		_FileHandle = CreateFile(outputFile.c_str(), // file name 
			GENERIC_WRITE,        // open for write 
			0,                    // do not share 
			NULL,                 // default security 
			CREATE_ALWAYS,        // overwrite existing
			FILE_ATTRIBUTE_NORMAL,// normal file 
			NULL);                // no template 

		if (_FileHandle == INVALID_HANDLE_VALUE)
		{
			cout << "Second CreateFile failed";
			return S_FALSE;
		}
		
		return S_OK;
	}

	REQUEST_NOTIFICATION_STATUS
	StartRead(
			IAsyncBufferFactory* BufferFactory,
			ULONG MaxBufferCount = MAX_ASYNC_FILE_BUFFER_COUNT,
			ULONG MaxBufferLength = ASYNC_FILE_BUFFER_SIZE_DEFAULT
		)
	{
		_AsyncReader.Init(
			BufferFactory,
			this,
			MaxBufferCount,
			MaxBufferLength
		);

		return _AsyncReader.StartRead();
	}

	REQUEST_NOTIFICATION_STATUS
	OnReadData(IAsyncBuffer* Buffer)
	{
		DWORD dwBytesWritten = 0;
		BOOL result;

		result = WriteFile(
			_FileHandle,
			Buffer->GetBuffer(),
			Buffer->GetValidLength(),
			&dwBytesWritten,
			NULL);

		if (result)
		{
			_FileLength += (ULONGLONG)Buffer->GetValidLength();
			return RQ_NOTIFICATION_CONTINUE;
		}

		cout << "Failed to WriteFile";
		return RQ_NOTIFICATION_FINISH_REQUEST;
	}

	REQUEST_NOTIFICATION_STATUS
	OnReadError(
			HRESULT ErrorCode
		)
	{
		Cleanup();
		cout << "OnReadError";
		return RQ_NOTIFICATION_FINISH_REQUEST;
	}

	REQUEST_NOTIFICATION_STATUS
	OnReadEOF()
	{
		BOOL validationFailed = FALSE;
		Cleanup();
		return RQ_NOTIFICATION_CONTINUE;
	}

	REQUEST_NOTIFICATION_STATUS
	OnAsyncCompletion(
		MyCompleletionInfo	*CompletionInfo
		)
	{
		return _AsyncReader.OnIoComplete(CompletionInfo);
	}

	HRESULT
	Cleanup()
	{
		if (_FileHandle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(_FileHandle);
			_FileHandle = INVALID_HANDLE_VALUE;
		}

		g_OutPutFileLength = _FileLength;
		return S_OK;
	}

	ASYNC_FILE_BUFFER_FACTORY _BufferFactory;
	HANDLE _FileHandle;
	ULONGLONG _FileLength = 0;

	///////////////////////////////////
	// Originally, this is a member of DAV_ASYNC_HANDLER
	///////////////////////////////////
	CAsyncReader _AsyncReader;

	///////////////////////////////////
	// jhkimtest temporary variables for simulating Handler
	///////////////////////////////////
	const int READBUFFERSIZE = g_ConfigurableBufferLength;
};
