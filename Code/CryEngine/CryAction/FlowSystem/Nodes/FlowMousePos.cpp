
//written by ToyaShiwasu

#include "StdAfx.h"
#include "FlowBaseNode.h"
#include "CryInput/IHardwareMouse.h"

class CInputGetMousePosNode : public CFlowBaseNode<eNCT_Singleton>
{
	enum EInPorts
	{
		Get,
	};
	enum EOutPorts
	{
		XPos,
		YPos,
	};
public:
	CInputGetMousePosNode(SActivationInfo * pActInfo)
	{

	}

	virtual void GetMemoryUsage(ICrySizer * s) const
	{
		s->Add(*this);
	}

	virtual void GetConfiguration(SFlowNodeConfig& config)
	{
		static const SInputPortConfig in_config[] =
		{
			InputPortConfig_Void("Get", _HELP("Get mouse cursor's position.")),
			{ 0 }
		};
		static const SOutputPortConfig out_config[] =
		{
			OutputPortConfig<int>("XPos", _HELP("X Position.")),
			OutputPortConfig<int>("YPos", _HELP("Y Position.")),
			{ 0 }
		};
		config.sDescription = _HELP("Get mouse cursor's position. When mouse is disabled, this node will return (-1, -1).");
		config.pInputPorts = in_config;
		config.pOutputPorts = out_config;
		config.SetCategory(EFLN_APPROVED);
	}

	virtual void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
	{
		switch (event)
		{
		case eFE_Activate:
			if (!IsPortActive(pActInfo, Get))
				break;

			float x=-1, y=-1;
			if (gEnv->pHardwareMouse)
				gEnv->pHardwareMouse->GetHardwareMouseClientPosition(&x, &y);
			ActivateOutput(pActInfo, XPos, (int)x);
			ActivateOutput(pActInfo, YPos, (int)y);
			break;
		}
	}
};

class CInputSetMousePosNode : public CFlowBaseNode<eNCT_Singleton>
{
	enum EInPorts
	{
		Set,
		XPos,
		YPos,
	};
public:
	CInputSetMousePosNode(SActivationInfo * pActInfo)
	{

	}

	virtual void GetMemoryUsage(ICrySizer * s) const
	{
		s->Add(*this);
	}

	virtual void GetConfiguration(SFlowNodeConfig& config)
	{
		static const SInputPortConfig in_config[] =
		{
			InputPortConfig_Void("Set", _HELP("Set mouse cursor's position. When mouse is disabled, this node will do nothing.")),
			InputPortConfig<int>("XPos", _HELP("X Position.")),
			InputPortConfig<int>("YPos", _HELP("Y Position")),
			{ 0 }
		};
		config.sDescription = _HELP("Set mouse cursor's position.");
		config.pInputPorts = in_config;
		config.SetCategory(EFLN_APPROVED);
	}

	virtual void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
	{
		switch (event)
		{
		case eFE_Activate:
			if (!IsPortActive(pActInfo, Set))
				break;
			if (gEnv->pHardwareMouse)
				gEnv->pHardwareMouse->SetHardwareMouseClientPosition((float)GetPortInt(pActInfo, XPos), (float)GetPortInt(pActInfo, YPos));
			break;
		}
	}

};

class CInputLockMousePosNode : public CFlowBaseNode<eNCT_Singleton>
{
	enum EInPorts
	{
		Lock,
		Unlock,
	};

public:
	CInputLockMousePosNode(SActivationInfo * pActInfo)
	{

	}

	~CInputLockMousePosNode()
	{
		//For abnormal termination.
#if CRY_PLATFORM_WINDOWS
		//Windows
		ClipCursor(NULL);
#elif CRY_PLATFORM_ORBIS
		//PS4
#elif CRY_PLATFORM_DURANGO
		//XboxOne
#endif		
	}

	virtual void GetMemoryUsage(ICrySizer * s) const
	{
		s->Add(*this);
	}

	virtual void GetConfiguration(SFlowNodeConfig& config)
	{
		static const SInputPortConfig in_config[] =
		{
			InputPortConfig_Void("Lock", _HELP("Lock mouse cursor's position.")),
			InputPortConfig_Void("Unlock", _HELP("Unlock mouse cursor's position.")),
			{ 0 }
		};
		static const SOutputPortConfig out_config[] =
		{
			{ 0 }
		};
		config.sDescription = _HELP("Lock/Unlock mouse cursor's position. This node is only for Windows. When other platforms, this node will do nothing.");
		config.pInputPorts = in_config;
		config.pOutputPorts = out_config;
		config.SetCategory(EFLN_APPROVED);
	}

	virtual void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
	{
		switch (event)
		{
		case eFE_Activate:
			if (IsPortActive(pActInfo, Lock))
			{
#if CRY_PLATFORM_WINDOWS
				//Windows
				POINT p;
				GetCursorPos(&p);
				RECT r;
				r.bottom = p.y;
				r.top = p.y;
				r.left = p.x;
				r.right = p.x;
				ClipCursor(&r);
#elif CRY_PLATFORM_ORBIS
				//PS4
#elif CRY_PLATFORM_DURANGO
				//XboxOne
#endif
				
			}
			if (IsPortActive(pActInfo, Unlock))
			{
#if CRY_PLATFORM_WINDOWS
				//Windows
				ClipCursor(NULL);
#elif CRY_PLATFORM_ORBIS
				//PS4
#elif CRY_PLATFORM_DURANGO
				//XboxOne
#endif
				
			}
			break;
		}
	}
};

REGISTER_FLOW_NODE("Input:GetMousePos", CInputGetMousePosNode);
REGISTER_FLOW_NODE("Input:SetMousePos", CInputSetMousePosNode);
REGISTER_FLOW_NODE("Input:LockMousePos", CInputLockMousePosNode);
