// dllmain.h : Declaration of module class.

class CTESTDLLCOMModule : public ATL::CAtlDllModuleT< CTESTDLLCOMModule >
{
public :
	DECLARE_LIBID(LIBID_TESTDLLCOMLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_TESTDLLCOM, "{ea30ef73-450c-47c3-a15b-56a491a90d55}")
};

extern class CTESTDLLCOMModule _AtlModule;
