#include <SADXModLoader.h>
#include "Objects.h"

FunctionPointer(void, DrawEmeraldCoastOcean, (OceanData *x), 0x004F8A30);
FunctionPointer(double, sub_789320, (float a2), 0x789320);

//Big ocean default UVs (Acts 2/3)
NJS_TEX uv_00CC0530_d[] = {
	{ 0, 255 },
	{ 0, -15045 },
	{ 15300, 255 },
	{ 15300, -15045 }
};

static float float1 = 0.02f; //These floats are used to patch repositioning of the high-poly ocean model
static float float2 = 66.0f; //These floats are used to patch repositioning of the high-poly ocean model
static int beachsea_water = 0; //Water animation
static bool inside_secret_area = false;
static bool SkyboxHidden = false;
static bool DynamicOceanModel = false; //Whether the dynamic ocean model is enabled
static NJS_VECTOR oldpos{ 0,0,0 }; //Vector used to calculate position difference
DataArray(NJS_TEX, uv_00CBB000_data, 0x10BB000, LengthOfArray(uv_00CBB000)); //Ocean model UVs
DataArray(FogData, EmeraldCoast1Fog, 0x00E99DDC, 3);
DataArray(FogData, EmeraldCoast2Fog, 0x00E99E0C, 3);
DataArray(FogData, EmeraldCoast3Fog, 0x00E99E3C, 3);
DataPointer(int, FramerateSetting, 0x0389D7DC);
DataPointer(float, EC1OceanYShift, 0x010C85A8);
DataPointer(int, DroppedFrames, 0x03B1117C);
DataPointer(int, CurrentFogToggle, 0x03ABDC6C);
DataPointer(float, CurrentFogLayer, 0x03ABDC60);
DataPointer(OceanData, OceanDataA, 0x03D0B8F0);
DataPointer(OceanData, OceanDataB, 0x03D0B90C);

int round(float r) {
	return (int)((r > 0.0) ? (r + 0.5) : (r - 0.5));
}

void __cdecl sub_4F76C0(ObjectMaster *a1) //Act 2
{
	float v1; // ST14_4@1
	double v2; // st7@1
	double v3; // st6@1
	DisableFog();
	njSetTexture((NJS_TEXLIST*)0x010C0508);
	DrawModelCallback_Queue((void(__cdecl *)(void *))DrawEmeraldCoastOcean, &OceanDataA, -17952.0, (QueuedModelFlagsB)0);
	v1 = OceanDataB.Position.x - Camera_Data1->Position.x;
	v2 = OceanDataB.Position.y - Camera_Data1->Position.y;
	v3 = OceanDataB.Position.z - Camera_Data1->Position.z;
	if (v3 * v3 + v2 * v2 + v1 * v1 < 9000000.0)
	{
		OceanDataB.Position.z = -2153.0f;
		DrawModelCallback_Queue((void(__cdecl *)(void *))DrawEmeraldCoastOcean, &OceanDataB, -17952.0, (QueuedModelFlagsB)0);
	}
}

void __cdecl sub_4F7760() //Act 3
{
	float v0; // ST14_4@1
	double v1; // st7@1
	double v2; // st6@1
	DisableFog();
	njSetTexture((NJS_TEXLIST*)0x010C0508);
	if (inside_secret_area == 0)
	{
		OceanDataA.Position.z = -800.0f;
		OceanDataA.Position.x = 4000.0f;
		DrawModelCallback_Queue((void(__cdecl *)(void *))DrawEmeraldCoastOcean, &OceanDataA, -37952.0, (QueuedModelFlagsB)0);
	}
	v0 = OceanDataB.Position.x - Camera_Data1->Position.x;
	v1 = OceanDataB.Position.y - Camera_Data1->Position.y;
	v2 = OceanDataB.Position.z - Camera_Data1->Position.z;
	if (v2 * v2 + v1 * v1 + v0 * v0 < 9000000.0)
	{
		OceanDataB.Position.y = -109.0f;
		DrawModelCallback_Queue((void(__cdecl *)(void *))DrawEmeraldCoastOcean, &OceanDataB, -37952.0, (QueuedModelFlagsB)0);
	}
}

void __cdecl Obj_EC1Water_DisplayX(ObjectMaster *a1) //Act 1
{
	DataPointer(float, flt_7E00DC, 0x7E00DC);
	DataPointer(float, flt_7DF1B0, 0x7DF1B0);
	auto entity = EntityData1Ptrs[0];
	double v2;
	int OceanUVShift1;
	DataArray(NJS_TEX, uv_00CC0530, 0x10C0530, 4);
	DataPointer(int, EffectActive, 0x3C5E4B0);
	DataPointer(int, FrameCounterUnpaused, 0x03ABDF5C);
	EntityData1 *v1; // esi@1
	v1 = a1->Data1;
	int unitsize_u_small = 10;
	int unitsize_v_small = 10;
	float u2_add;
	float v2_add;
	int u2_delta;
	int v2_delta;
	DisableFog();
	if (*(signed int*)&v1->CharIndex)
	{
		v2 = njSin(FrameCounterUnpaused << 11) * 1.5f + 0.2f;
		EC1OceanYShift = v2;
	}
	else
	{
		if (EC1OceanYShift > -1.5f)
		{
			EC1OceanYShift = EC1OceanYShift - 0.1f;
		}
	}
	if (entity != nullptr)
	{
		if (entity->Position.x > 1800)
		{
			DynamicOceanModel = true;
			WriteData((float**)0x00501824, &flt_7E00DC);
			WriteData((float**)0x00501849, &flt_7E00DC);
			WriteData((float**)0x00501832, &flt_7DF1B0);
			WriteData((float**)0x0050185B, &flt_7DF1B0);
		}
		if (entity->Position.x < 1400) DynamicOceanModel = false;
	}
	if (DynamicOceanModel == false)
	{
		njSetTexture((NJS_TEXLIST*)0x010C0508);
		if (v1->CharIndex)
		{
			v2 = njSin(FrameCounterUnpaused << 11) * 3.0f + 2.5f;
			EC1OceanYShift = v2;
			OceanDataA.Position.y = v2;
		}
		else
		{
			if (EC1OceanYShift > -1.5f)
			{
				EC1OceanYShift = EC1OceanYShift - 0.1f;
			}
			OceanDataA.Position.y = EC1OceanYShift;
		}
		DrawModelCallback_Queue((void(__cdecl *)(void *))DrawEmeraldCoastOcean, &OceanDataA, -17952.0, (QueuedModelFlagsB)0);
	}
		else
		{
			if (!DroppedFrames)
			{
				njSetTexture((NJS_TEXLIST*)0x010C0508); //BEACH_SEA
				if (oldpos.x != v1->Position.x)
				{
					u2_add = int(255 * (v1->Position.x - oldpos.x) / unitsize_u_small) % 255;
					u2_add = round(1.5f * u2_add);
					for (unsigned int u_step = 0; u_step < LengthOfArray(uv_00CBB000_d); u_step++)
					{
						uv_00CBB000_data[u_step].u = uv_00CBB000_data[u_step].u - u2_add;
						u2_delta = uv_00CBB000_data[u_step].u - uv_00CBB000_d[u_step].u;
						uv_00CBB000_data[u_step].u = uv_00CBB000_d[u_step].u + (u2_delta % 255);
						uv_00CBB000[u_step].u = uv_00CBB000_data[u_step].u;
					}
				}
				if (oldpos.z != v1->Position.z)
				{
					v2_add = int(255 * (v1->Position.z - oldpos.z) / unitsize_v_small) % 255;
					v2_add = round(0.5f * v2_add);
					for (unsigned int v_step = 0; v_step < LengthOfArray(uv_00CBB000_d); v_step++)
					{
						uv_00CBB000_data[v_step].v = uv_00CBB000_data[v_step].v - v2_add;
						v2_delta = uv_00CBB000_data[v_step].v - uv_00CBB000_d[v_step].v;
						uv_00CBB000_data[v_step].v = uv_00CBB000_d[v_step].v + (v2_delta % 255);
						uv_00CBB000[v_step].v = uv_00CBB000_data[v_step].v;
					}
				}
				oldpos.x = v1->Position.x;
				oldpos.z = v1->Position.z;
				njPushMatrix(0);
				njTranslate(0, v1->Position.x, EC1OceanYShift, v1->Position.z);
				if (EC1OceanYShift > -1.5f) ProcessModelNode_D_Wrapper((NJS_OBJECT*)0x010C03FC, 1.0f);
				else ProcessModelNode_A_Wrapper((NJS_OBJECT*)0x010C03FC, QueuedModelFlagsB_3, 1.0f);
				njPopMatrix(1u);

				njPushMatrix(0);
				njTranslate(0, v1->Position.x - 2000, EC1OceanYShift, v1->Position.z);
				ProcessModelNode_A_Wrapper(&object_00CC03FC, QueuedModelFlagsB_3, 1.0f);
				njPopMatrix(1u);

				njPushMatrix(0);
				njTranslate(0, v1->Position.x - 2000, EC1OceanYShift, v1->Position.z + 1000);
				ProcessModelNode_A_Wrapper(&object_00CC03FC, QueuedModelFlagsB_3, 1.0f);
				njPopMatrix(1u);

				njPushMatrix(0);
				njTranslate(0, v1->Position.x - 2000, EC1OceanYShift, v1->Position.z - 1000);
				ProcessModelNode_A_Wrapper(&object_00CC03FC, QueuedModelFlagsB_3, 1.0f);
				njPopMatrix(1u);

				njPushMatrix(0);
				njTranslate(0, v1->Position.x, EC1OceanYShift, v1->Position.z - 2000);
				ProcessModelNode_A_Wrapper(&object_00CC03FC, QueuedModelFlagsB_3, 1.0f);
				njPopMatrix(1u);

				njPushMatrix(0);
				njTranslate(0, v1->Position.x - 1000, EC1OceanYShift, v1->Position.z - 2000);
				ProcessModelNode_A_Wrapper(&object_00CC03FC, QueuedModelFlagsB_3, 1.0f);
				njPopMatrix(1u);

				njPushMatrix(0);
				njTranslate(0, v1->Position.x + 1000, EC1OceanYShift, v1->Position.z - 2000);
				ProcessModelNode_A_Wrapper(&object_00CC03FC, QueuedModelFlagsB_3, 1.0f);
				njPopMatrix(1u);

				njPushMatrix(0);
				njTranslate(0, v1->Position.x + 1000, EC1OceanYShift, v1->Position.z);
				ProcessModelNode_A_Wrapper(&object_00CC03FC, QueuedModelFlagsB_3, 1.0f);
				njPopMatrix(1u);

				njPushMatrix(0);
				njTranslate(0, v1->Position.x + 2000, EC1OceanYShift, v1->Position.z);
				ProcessModelNode_A_Wrapper(&object_00CC03FC, QueuedModelFlagsB_3, 1.0f);
				njPopMatrix(1u);

				njPushMatrix(0);
				njTranslate(0, v1->Position.x + 2000, EC1OceanYShift, v1->Position.z + 1000);
				ProcessModelNode_A_Wrapper(&object_00CC03FC, QueuedModelFlagsB_3, 1.0f);
				njPopMatrix(1u);

				njPushMatrix(0);
				njTranslate(0, v1->Position.x + 2000, EC1OceanYShift, v1->Position.z - 1000);
				ProcessModelNode_A_Wrapper(&object_00CC03FC, QueuedModelFlagsB_3, 1.0f);
				njPopMatrix(1u);

				njPushMatrix(0);
				njTranslate(0, v1->Position.x, EC1OceanYShift, v1->Position.z + 2000);
				ProcessModelNode_A_Wrapper(&object_00CC03FC, QueuedModelFlagsB_3, 1.0f);
				njPopMatrix(1u);

				njPushMatrix(0);
				njTranslate(0, v1->Position.x + 1000, EC1OceanYShift, v1->Position.z + 2000);
				ProcessModelNode_A_Wrapper(&object_00CC03FC, QueuedModelFlagsB_3, 1.0f);
				njPopMatrix(1u);

				njPushMatrix(0);
				njTranslate(0, v1->Position.x - 1000, EC1OceanYShift, v1->Position.z + 2000);
				ProcessModelNode_A_Wrapper(&object_00CC03FC, QueuedModelFlagsB_3, 1.0f);
				njPopMatrix(1u);

				njPushMatrix(0);
				njTranslate(0, v1->Position.x - 1000, EC1OceanYShift, v1->Position.z);
				ProcessModelNode_A_Wrapper(&object_00CC03FC, QueuedModelFlagsB_3, 1.0f);
				njPopMatrix(1u);

				njPushMatrix(0);
				njTranslate(0, v1->Position.x, EC1OceanYShift, v1->Position.z + 1000);
				ProcessModelNode_A_Wrapper(&object_00CC03FC, QueuedModelFlagsB_3, 1.0f);
				njPopMatrix(1u);

				njPushMatrix(0);
				njTranslate(0, v1->Position.x, EC1OceanYShift, v1->Position.z - 1000);
				ProcessModelNode_A_Wrapper(&object_00CC03FC, QueuedModelFlagsB_3, 1.0f);
				njPopMatrix(1u);

				njPushMatrix(0);
				njTranslate(0, v1->Position.x + 1000, EC1OceanYShift, v1->Position.z + 1000);
				ProcessModelNode_A_Wrapper(&object_00CC03FC, QueuedModelFlagsB_3, 1.0f);
				njPopMatrix(1u);

				njPushMatrix(0);
				njTranslate(0, v1->Position.x + 1000, EC1OceanYShift, v1->Position.z - 1000);
				ProcessModelNode_A_Wrapper(&object_00CC03FC, QueuedModelFlagsB_3, 1.0f);
				njPopMatrix(1u);

				njPushMatrix(0);
				njTranslate(0, v1->Position.x - 1000, EC1OceanYShift, v1->Position.z + 1000);
				ProcessModelNode_A_Wrapper(&object_00CC03FC, QueuedModelFlagsB_3, 1.0f);
				njPopMatrix(1u);

				njPushMatrix(0);
				njTranslate(0, v1->Position.x - 1000, EC1OceanYShift, v1->Position.z - 1000);
				ProcessModelNode_A_Wrapper(&object_00CC03FC, QueuedModelFlagsB_3, 1.0f);
				njPopMatrix(1u);
			}
		}
	}

	void EC1WaterAnimation_SADX()
	{
		if (beachsea_water <= 14) njSetTextureNum(beachsea_water);
	}

	extern "C" __declspec(dllexport) void __cdecl Init()
	{
		HMODULE handle = GetModuleHandle(L"DC_EmeraldCoast");
		if (handle == 0)
		{
			for (int i = 0; i < 3; i++)
			{
				SkyboxScale_EmeraldCoast1[i].x = 1.0f;
				SkyboxScale_EmeraldCoast1[i].y = 1.0f;
				SkyboxScale_EmeraldCoast1[i].z = 1.0f;
				SkyboxScale_EmeraldCoast2[i].x = 1.0f;
				SkyboxScale_EmeraldCoast2[i].y = 1.0f;
				SkyboxScale_EmeraldCoast2[i].z = 1.0f;
				SkyboxScale_EmeraldCoast3[i].x = 1.0f;
				SkyboxScale_EmeraldCoast3[i].y = 1.0f;
				SkyboxScale_EmeraldCoast3[i].z = 1.0f;
				DrawDist_EmeraldCoast1[i].Maximum = -6000.0f;
				DrawDist_EmeraldCoast2[i].Maximum = -3900.0f;
				DrawDist_EmeraldCoast3[i].Maximum = -4000.0f;
				EmeraldCoast1Fog[i].Distance = -10000.0f;
				EmeraldCoast1Fog[i].Layer = -1.0f;
				EmeraldCoast2Fog[i].Distance = -10000.0f;
				EmeraldCoast2Fog[i].Layer = -1.0f;
				EmeraldCoast3Fog[i].Toggle = 0;
				EmeraldCoast3Fog[i].Layer = -1200.0f;
				EmeraldCoast3Fog[i].Distance = -3000.0f;
				EmeraldCoast3Fog[i].Color = 0xFFFFFFFF;
			}
			WriteData<2>((void*)0x004F8A9A, 0x90); //Disable built-in water animation in Act 1
			WriteData<2>((char*)0x004F7816, 0xFF); //Disable built-in water animation in Act 2
			WriteData<2>((char*)0x004F78E6, 0xFF); //Disable built-in water animation in Act 3
			WriteCall((void*)0x004F8B23, EC1WaterAnimation_SADX); //Sea animation in Acts 1/2
			ResizeTextureList((NJS_TEXLIST*)0x010C0508, 32); //BEACH_SEA texlist
			DataArray(PVMEntry, BeachTexlists, 0x0102F408, 32);
			BeachTexlists[1].Name = "BEACH_SEAWX";
			for (unsigned int rq = 0; rq < LengthOfArray(uv_00CBB000_d); rq++)
			{
				uv_00CBB000_d[rq].u = round(0.5 * uv_00CBB000_d[rq].u);
				uv_00CBB000_d[rq].v = round(0.5 * uv_00CBB000_d[rq].v);
			}
			((NJS_OBJECT*)0x10C05E8)->basicdxmodel->mats[0].attrflags &= ~NJD_DA_INV_SRC;
			((NJS_OBJECT*)0x10C05E8)->basicdxmodel->mats[0].attrflags |= NJD_DA_ONE;
			((NJS_OBJECT*)0x10C05E8)->basicdxmodel->mats[0].attrflags |= NJD_SA_SRC;
			//((NJS_OBJECT*)0x10C05E8)->basicdxmodel->mats[0].attrflags |= NJD_FLAG_IGNORE_LIGHT;
			((NJS_OBJECT*)0x10C05E8)->basicdxmodel->mats[0].diffuse.color = 0xFFFFFFFF;
			((NJS_OBJECT*)0x010C03FC)->basicdxmodel->mats[0].attrflags &= ~NJD_DA_INV_SRC;
			((NJS_OBJECT*)0x010C03FC)->basicdxmodel->mats[0].attrflags |= NJD_DA_ONE;
			((NJS_OBJECT*)0x010C03FC)->basicdxmodel->mats[0].attrflags |= NJD_SA_SRC;
			//((NJS_OBJECT*)0x010C03FC)->basicdxmodel->mats[0].attrflags |= NJD_FLAG_IGNORE_LIGHT;
			((NJS_OBJECT*)0x010C03FC)->basicdxmodel->mats[0].diffuse.color = 0xFFFFFFFF;

			object_00CC03FC.basicdxmodel->mats[0].attrflags &= ~NJD_DA_INV_SRC;
			object_00CC03FC.basicdxmodel->mats[0].attrflags |= NJD_DA_ONE;
			object_00CC03FC.basicdxmodel->mats[0].attrflags |= NJD_SA_SRC;
			//object_00CC03FC.basicdxmodel->mats[0].attrflags |= NJD_FLAG_IGNORE_LIGHT;
			object_00CC03FC.basicdxmodel->mats[0].diffuse.color = 0xFFFFFFFF;
			WriteData<5>((void*)0x004F76DE, 0x90); //Kill SADX ocean water in Act 2
			WriteData<5>((void*)0x004F777E, 0x90); //Kill SADX ocean water in Act 3
			WriteData<1>((void*)0x004F783A, 0x0F); //15 animation frames for water in Act 2
			WriteData<1>((void*)0x004F790A, 0x0F); //15 animation frames for water in Act 3
			//Write floats to fix buggy SADX water positioning code
			//Act 2
			WriteData((float**)0x004F7876, &float1);
			WriteData((float**)0x004F7891, &float1);
			WriteData((float**)0x004F78A4, &float2);
			WriteData((float**)0x004F78B1, &float2);
			//Act 3
			WriteData((float**)0x004F795D, &float1);
			WriteData((float**)0x004F7978, &float1);
			WriteData((float**)0x004F798B, &float2);
			WriteData((float**)0x004F7998, &float2);
			//Write water rendering functions
			WriteJump((void*)0x00501130, Obj_EC1Water_DisplayX); //Act 1
			WriteJump((void*)0x004F76C0, sub_4F76C0); //Act 2
			WriteJump((void*)0x004F7760, sub_4F7760); //Act 3
		}
	}


	extern "C" __declspec(dllexport) void __cdecl OnFrame()
	{
		HMODULE handle = GetModuleHandle(L"DC_EmeraldCoast");
		if (handle == 0)
		{
			Col_ECoast1[455].Flags = 0x80040000; //Prevent some seashore object from clipping
			//Hide skybox bottom in Act 3
			if (CurrentLevel == 1 && CurrentAct == 2 && Camera_Data1 != nullptr)
			{
				if (Camera_Data1->Position.y < 50 && SkyboxHidden == false)
				{
					((NJS_OBJECT *)0x103B37C)->evalflags |= NJD_EVAL_HIDE;
					SkyboxHidden = true;
				}
				if (Camera_Data1->Position.y >= 50 && SkyboxHidden == true)
				{
					((NJS_OBJECT *)0x103B37C)->evalflags &= ~NJD_EVAL_HIDE;
					SkyboxHidden = false;
				}
			}
			if (CurrentLevel == 1 && GameState != 16)
			{
				//Restore ocean UVs on level exit/restart
				if (GameState == 3 || GameState == 4 || GameState == 7 || GameState == 21)
				{
					inside_secret_area = 0;
					for (unsigned int r2 = 0; r2 < LengthOfArray(uv_00CBB000_d); r2++)
					{
						uv_00CBB000_data[r2].u = uv_00CBB000_d[r2].u;
						uv_00CBB000_data[r2].v = uv_00CBB000_d[r2].v;
						uv_00CBB000[r2].u = uv_00CBB000_d[r2].u;
						uv_00CBB000[r2].v = uv_00CBB000_d[r2].v;
					}
				}
				if (beachsea_water > 14) beachsea_water = 0;
				((NJS_OBJECT*)0x010C03FC)->basicdxmodel->mats[0].attr_texId = 17 + beachsea_water;
				((NJS_OBJECT*)0x10C05E8)->basicdxmodel->mats[0].attr_texId = 17 + beachsea_water;
				matlist_00CBA58C[0].attr_texId = 17 + beachsea_water;
				if (FramerateSetting < 2 && FrameCounterUnpaused % 4 == 0 || FramerateSetting == 2 && FrameCounterUnpaused % 2 == 0 || FramerateSetting > 2) beachsea_water++;
			}
			if (CurrentLevel == 1 && CurrentAct == 2 && GameState != 16)
			{
				if (GameState == 3 || GameState == 4 || GameState == 7 || GameState == 21)
				{
					inside_secret_area = false;
					CurrentFogToggle = 0;
					WriteData<1>((void*)0x004F777E, 0xE8); //Restore the ocean
					WriteData<1>((void*)0x004F777F, 0xBD); //Restore the ocean
					WriteData<1>((void*)0x004F7780, 0xD0); //Restore the ocean
					WriteData<1>((void*)0x004F7781, 0xF0); //Restore the ocean
					WriteData<1>((void*)0x004F7782, 0xFF); //Restore the ocean
				}
				if (Camera_Data1 != nullptr && Camera_Data1->Position.z > 2000)
				{
					inside_secret_area = 1;
					CurrentFogToggle = 1;
					if (CurrentFogLayer < -21) CurrentFogLayer = CurrentFogLayer + 20;
					WriteData<5>((void*)0x004F777E, 0x90); //Kill the ocean temporarily
				}
				else
				{
					if (CurrentFogLayer > -1200) CurrentFogLayer = CurrentFogLayer - 20;
					if (CurrentFogLayer <= -1200 && CurrentFogToggle == 1)
					{
						inside_secret_area = 0;
						CurrentFogToggle = 0;
						WriteData<1>((void*)0x004F777E, 0xE8); //Restore the ocean
						WriteData<1>((void*)0x004F777F, 0xBD); //Restore the ocean
						WriteData<1>((void*)0x004F7780, 0xD0); //Restore the ocean
						WriteData<1>((void*)0x004F7781, 0xF0); //Restore the ocean
						WriteData<1>((void*)0x004F7782, 0xFF); //Restore the ocean
					}
				}
			}
		}
	}

	extern "C" __declspec(dllexport) const ModInfo SADXModInfo = { ModLoaderVer };