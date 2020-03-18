#include "common.hpp"

namespace CTRPluginFramework
{
	
	typedef struct {
		uint8_t r[32];
		uint8_t g[32];
		uint8_t b[32];
	} LED;
	
	typedef struct {
		// Animation effect parameters
		// https://www.3dbrew.org/wiki/MCURTC:SetInfoLEDPattern
		uint8_t delay = 0x00;
		uint8_t smoothing = 0x00;
		uint8_t loop_delay = 0xFF;
		
		// Color parameters
		uint8_t r[32];
		uint8_t g[32];
		uint8_t b[32];
	} LED_MCU;
	
	class PtmLedFlashStruct
	{
	public:
		MenuEntry* entry;
		Handle serviceHandle = 0;
		LED RGB_DATA;
		LED_MCU MCU_PAT;
		
		PtmLedFlashStruct(MenuEntry* _InitEntry)
		{
			entry = _InitEntry;
		}
		
		void PtmSysmSetInfoLedPattern(LED_MCU *data)
		{
			Result result = srvGetServiceHandle(&serviceHandle, "ptm:sysm");
			if (result != 0) 
			{
				entry->Disable();
				return;
			}

			u32* prm = getThreadCommandBuffer();
			prm[0] = 0x8010640;
			memcpy(&prm[1], data, 0x64);
			svcSendSyncRequest(serviceHandle);
			svcCloseHandle(serviceHandle);
		}
		
		void PtmLedFlash(int r, int g, int b)
		{
			memset(&RGB_DATA.r[0], r, 31); 
			memset(&RGB_DATA.g[0], g, 31); 
			memset(&RGB_DATA.b[0], b, 31);
		
			for(int i=0; i<=31; i++)
			{
				MCU_PAT.r[i] = RGB_DATA.r[i];
				MCU_PAT.g[i] = RGB_DATA.g[i];
				MCU_PAT.b[i] = RGB_DATA.b[i];
			}
		
			PtmLedFlashStruct::PtmSysmSetInfoLedPattern(&MCU_PAT);
		}
		
		void DisableLED(void)
		{
			PtmLedFlash(0, 0, 0);
		}
	};
	
	void LedFlah(MenuEntry* entry)
	{
		int r = 0, g = 0, b = 0;
		
		// PTM:SYSMサービスハンドルの取得に失敗した場合に無効化するエントリ
		PtmLedFlashStruct PTM_S(entry);
		
		if(Controller::IsKeyDown(Key::Y))
		{
			// スムージングの大きさ
			PTM_S.MCU_PAT.smoothing = 0x20;
		}
		
		if(Controller::IsKeyDown(Key::DPadLeft))
		{
			// 赤
			r = 0xFF;
			PTM_S.PtmLedFlash(r, g, b);
		}
		
		else if(Controller::IsKeyDown(Key::DPadDown))
		{
			// 緑
			g = 0xFF;
			PTM_S.PtmLedFlash(r, g, b);
		}
		
		else if(Controller::IsKeyDown(Key::DPadRight))
		{
			// 青
			b = 0xFF;
			PTM_S.PtmLedFlash(r, g, b);
		}
		
		else if(Controller::IsKeyDown(Key::DPadUp))
		{
			// 白
			r = 0xFF;
			g = 0xFF;
			b = 0xFF;
			PTM_S.PtmLedFlash(r, g, b);
		}
		
		// LEDを消灯する(バグあり)
		else PTM_S.DisableLED();
	}
}