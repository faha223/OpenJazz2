#include "Animation.h"
#include "FileSystem.h"
#include <SDL2/SDL.h>

#ifndef _WIN32
#define sprintf_s snprintf
#endif
using namespace std;

static uint32_t FrameCount = 0;

#pragma region Palettes

static uint32_t palette1[256] = {	0x00969696, 0x00010000, 0x00000100, 0x00000001, 0x00010100, 0x00010001, 0x00000101, 0x00010101, 0x00020000, 0x00020100, 0x00C343FF, 0x00BCB0AE, 0x00E0ED3F, 0x00DA99A0, 0x00322624, 0x00FEFFFF,
									0x0000FFC7, 0x0000DF93, 0x0000BF6B, 0x0000A347, 0x0000832B, 0x00006713, 0x00003707, 0x00000C00, 0x000000FF, 0x000000E3, 0x000000C7, 0x000000AB, 0x0000008F, 0x00000073, 0x0000003F, 0x0000000C,
									0x00FFE3BB, 0x00FFC77B, 0x00FFAB3B, 0x00FF8B00, 0x00CB6B00, 0x00974F00, 0x004F2F00, 0x000B0700, 0x0000FFFF, 0x0000C7FF, 0x000093FF, 0x00005FFF, 0x000037CB, 0x00001B9B, 0x00000753, 0x0000000B,
									0x00B78BFB, 0x00975BF7, 0x007B2BF3, 0x006300EF, 0x004B00BF, 0x00370093, 0x00230063, 0x00130037, 0x00B5B038, 0x00C9DB4E, 0x00F5C6CA, 0x0000C3DB, 0x000093BB, 0x00006B9B, 0x00003753, 0x0000070B,
									0x00D3F3FF, 0x00AFCFDB, 0x0093AFBB, 0x00738B9B, 0x00576B77, 0x003F4B57, 0x001F232F, 0x0007070B, 0x00FFE7D3, 0x00DBC3AB, 0x00BB9F8B, 0x009B7F6B, 0x00775F4B, 0x00573F33, 0x002F1F1B, 0x000B0707,
									0x00A3FF00, 0x007FE300, 0x005FC707, 0x0043AB07, 0x002F8F0B, 0x001F770B, 0x00073F00, 0x00000B00, 0x00FF63DB, 0x00FF2FCF, 0x00FF00C7, 0x00CF009F, 0x009F0077, 0x006F0053, 0x003F002F, 0x000F000B,
									0x008F00CF, 0x008300BF, 0x007B00AF, 0x006F07A3, 0x00670793, 0x005B0787, 0x00530777, 0x00470767, 0x003F075B, 0x0037074F, 0x00330043, 0x002B0037, 0x0023002B, 0x001B001F, 0x000F0013, 0x00070007,
									0x00FF3BD3, 0x00EF2BB7, 0x00DF1F9F, 0x00CF1787, 0x00BF0B6F, 0x00AF075B, 0x009F0047, 0x008F0037, 0x007F0027, 0x006F001F, 0x005F0013, 0x004F000B, 0x003F0007, 0x002F0000, 0x001F0000, 0x000F0000,
									0x00F7FF00, 0x00DFE700, 0x00CBCF00, 0x00B3B700, 0x009B9F00, 0x00838700, 0x006F6F00, 0x00636300, 0x00575700, 0x004F4F00, 0x00434300, 0x00383700, 0x002F2F00, 0x00232300, 0x00171700, 0x000F0F00,
									0x00C71B77, 0x00BB1767, 0x00AF1357, 0x00A70F47, 0x009B0F3B, 0x00930B2F, 0x00870B23, 0x007B071B, 0x0073070F, 0x00670707, 0x005F0000, 0x00530700, 0x00470700, 0x003F0B00, 0x00330B00, 0x002B0C00,
									0x00AF8B00, 0x00A38300, 0x00977F00, 0x008F7700, 0x00836F00, 0x00776700, 0x006F5F00, 0x00635700, 0x00574F00, 0x004F4700, 0x00433F00, 0x00373700, 0x00B7B75F, 0x00D6838D, 0x001CB1CF, 0x00B3A841,
									0x00FB476B, 0x00F34363, 0x00EF435F, 0x00E73F5B, 0x00E33B53, 0x00DB374F, 0x00D33747, 0x00CF3343, 0x00C7333F, 0x00C32F3B, 0x00BB2B37, 0x00B72B33, 0x00AF272B, 0x00AB272B, 0x00A32327, 0x009F2323,
									0x00971F1F, 0x00931F1F, 0x008B1F1B, 0x00871F1B, 0x007F1F17, 0x007B1F17, 0x006F1F13, 0x00671B0F, 0x005B170B, 0x004F1307, 0x00471307, 0x003F0F00, 0x00370B00, 0x002B0B00, 0x00230700, 0x001B0700,
									0x00DCE278, 0x00C1CD52, 0x00706462, 0x00E8FB3C, 0x00A9BB60, 0x00FFFFBF, 0x00FFFFD1, 0x00989840, 0x00FFFFCB, 0x00DA9379, 0x00219ED1, 0x00AB6A71, 0x00A69A98, 0x00C59073, 0x00FFFFD6, 0x000BBEB6,
									0x00FCD3C5, 0x0097921A, 0x00FFFFCF, 0x0076474B, 0x00FA7D9E, 0x00BE9B7B, 0x00D6D67E, 0x00D3C7C5, 0x00B9A821, 0x009E4B55, 0x00788A2F, 0x00A2AE65, 0x00A45D43, 0x00BCC91B, 0x00E0817C, 0x00A33856,
									0x0090851E, 0x00FCC1B6, 0x00BDC359, 0x00FFFFAD, 0x008D6456, 0x001974DE, 0x00010200, 0x00020200, 0x00020001, 0x00010002, 0x00020002, 0x00000201, 0x00000102, 0x00000202, 0x00020101, 0x00FFFFFE };

static uint32_t palette2[256] = {	0x00969696, 0x00010000, 0x00000100, 0x00000001, 0x00010100, 0x00010001, 0x00000101, 0x00010101, 0x00020000, 0x00020100, 0x005F737F, 0x00778F9F, 0x0097AFBF, 0x00B3CFDF, 0x00D3F3FF, 0x00FEFFFF,
									0x00DFFFF7, 0x00B7FFEB, 0x0093FFE3, 0x006FFFDB, 0x0000FFC7, 0x0000E7A3, 0x0000CF7F, 0x0000B763, 0x00009F4B, 0x00008B27, 0x0000770B, 0x00076300, 0x00134F00, 0x00173B00, 0x00172700, 0x00101300,
									0x00EBFFFB, 0x00D7FFFB, 0x00C7FFFB, 0x00B7F7FF, 0x00A7EFFF, 0x0097E3FF, 0x0087D3FF, 0x0077C3FF, 0x0073B3DF, 0x006BA7C3, 0x005F93A3, 0x00577B87, 0x0047636B, 0x00374B4B, 0x00232F2F, 0x000F1313,
									0x00FFEFEB, 0x00FBCFBF, 0x00F7B39B, 0x00F39B73, 0x00EF834F, 0x00EB732B, 0x00DB6713, 0x00CB5B00, 0x00B35300, 0x009F4B00, 0x00874700, 0x00733F00, 0x005F3300, 0x00472B00, 0x00331F00, 0x001F1400,
									0x00DFDFFF, 0x00B7B7FF, 0x009393FF, 0x006F6FFF, 0x004747FF, 0x002323FF, 0x000000FF, 0x000B00E3, 0x002300C7, 0x003700AF, 0x00470093, 0x004B007B, 0x0047005F, 0x003F0047, 0x002B0027, 0x0013000F,
									0x00DFFBFF, 0x0093F3FF, 0x0047EFFF, 0x003BD7FF, 0x002FC3FF, 0x0023A7FF, 0x00178FFF, 0x002F73EB, 0x004367DB, 0x00575FCB, 0x003B53AB, 0x0027478B, 0x00173B6F, 0x000B2F4F, 0x0000232F, 0x00000F13,
									0x0043FF00, 0x004BEB00, 0x0053D700, 0x0057C700, 0x005BB300, 0x005BA300, 0x00579300, 0x004F8300, 0x004B7700, 0x00436700, 0x003B5B00, 0x00334B00, 0x002B3B00, 0x00232F00, 0x00171F00, 0x000F1300,
									0x00FFDFFB, 0x00FFAFF3, 0x00FF83EB, 0x00FF57E7, 0x00FF2BDF, 0x00FF00D7, 0x00E700BF, 0x00CF00AB, 0x00B70097, 0x009F0083, 0x0087006F, 0x006F005B, 0x00570047, 0x003F0033, 0x002B001F, 0x00130010,
									0x00FFFBF7, 0x00EBE3D7, 0x00DBCBB7, 0x00CBB39F, 0x00B79F87, 0x00A78B6F, 0x00977B5B, 0x008B6B4B, 0x007F5F3B, 0x0073532F, 0x00674723, 0x005B3F1B, 0x004B330F, 0x003B2707, 0x002B1B00, 0x001F1300,
									0x00FF00DF, 0x00F700D7, 0x00F300CF, 0x00EB00C7, 0x00E300BF, 0x00DF00B7, 0x00D700AF, 0x00D300A7, 0x00CB019F, 0x00C7009B, 0x00BF0093, 0x00BB008F, 0x00B30087, 0x00AB007F, 0x00A7007B, 0x009F0073,
									0x009B016F, 0x00930067, 0x008F0063, 0x0087005B, 0x00830057, 0x007B0053, 0x0077004B, 0x006F0047, 0x00670043, 0x0063003B, 0x005B0037, 0x00570033, 0x004F002F, 0x004B002B, 0x00430027, 0x003F0023,
									0x00CB27D7, 0x00BB23C7, 0x00AF1FB7, 0x00A31FAB, 0x00931B9B, 0x0087178F, 0x0077177F, 0x006B1373, 0x005F0F63, 0x004F0F53, 0x00430B47, 0x00330737, 0x0027072B, 0x001B001F, 0x000F0013, 0x00070007,
									0x00FFFEFF, 0x00D7FFFF, 0x00B3FFFF, 0x008FFFFF, 0x006BFFFF, 0x0047FFFF, 0x0023FFFF, 0x0000FFFF, 0x0000F7F7, 0x0000EFEF, 0x0000E7E7, 0x0000DFDF, 0x0000D7D7, 0x0000CFCF, 0x0000C7C7, 0x0000BFBF,
									0x00570023, 0x005F0027, 0x0063002B, 0x006B002F, 0x00730033, 0x007B003B, 0x0083003F, 0x008B0043, 0x0093004B, 0x009B004F, 0x00A30057, 0x00AB005B, 0x00B30063, 0x00BB0067, 0x00C3006F, 0x00CB0077,
									0x00570133, 0x005F0037, 0x0063003F, 0x006B0043, 0x0073004B, 0x007B004F, 0x00830058, 0x008B005F, 0x00930068, 0x009B006F, 0x00A30073, 0x00AB007B, 0x00B30083, 0x00BB008B, 0x00C30097, 0x00CB009F,
									0x00C3FF00, 0x00A7E300, 0x008FC700, 0x007BAB00, 0x00638F00, 0x004F7300, 0x00FFD3F7, 0x00FFD4F7, 0x00FFD3F8, 0x00FFD4F8, 0x00FFD3F6, 0x00FFD4F6, 0x00FFD2F6, 0x00FFD2F7, 0x00FFD2F8, 0x00FFFFFE };

static uint32_t palette3[256] = {	0x00000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000,
									0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000,
									0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000,
									0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000,
									0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000,
									0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000,
									0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000,
									0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000, 0x7F000000,
									0x7F010101, 0x7F030303, 0x7F050505, 0x7F070707, 0x7F090909, 0x7F0B0B0B, 0x7F0D0D0D, 0x7F0F0F0F, 0x7F111111, 0x7F131313, 0x7F151515, 0x7F171717, 0x7F191919, 0x7F1B1B1B, 0x7F1D1D1D, 0x7F1F1F1F,
									0x7F212121, 0x7F232323, 0x7F252525, 0x7F272727, 0x7F292929, 0x7F2B2B2B, 0x7F2D2D2D, 0x7F2F2F2F, 0x7F313131, 0x7F333333, 0x7F353535, 0x7F373737, 0x7F393939, 0x7F3B3B3B, 0x7F3D3D3D, 0x7F3F3F3F,
									0x7F414141, 0x7F434343, 0x7F454545, 0x7F474747, 0x7F494949, 0x7F4B4B4B, 0x7F4D4D4D, 0x7F4F4F4F, 0x7F515151, 0x7F535353, 0x7F555555, 0x7F575757, 0x7F595959, 0x7F5B5B5B, 0x7F5D5D5D, 0x7F5F5F5F,
									0x7F616161, 0x7F636363, 0x7F656565, 0x7F676767, 0x7F696969, 0x7F6B6B6B, 0x7F6D6D6D, 0x7F6F6F6F, 0x7F717171, 0x7F737373, 0x7F757575, 0x7F777777, 0x7F797979, 0x7F7B7B7B, 0x7F7D7D7D, 0x7F7F7F7F,
									0x7F818181, 0x7F838383, 0x7F858585, 0x7F878787, 0x7F898989, 0x7F8B8B8B, 0x7F8D8D8D, 0x7F8F8F8F, 0x7F919191, 0x7F939393, 0x7F959595, 0x7F979797, 0x7F999999, 0x7F9B9B9B, 0x7F9D9D9D, 0x7F9F9F9F,
									0x7FA1A1A1, 0x7FA3A3A3, 0x7FA5A5A5, 0x7FA7A7A7, 0x7FA9A9A9, 0x7FABABAB, 0x7FADADAD, 0x7FAFAFAF, 0x7FB1B1B1, 0x7FB3B3B3, 0x7FB5B5B5, 0x7FB7B7B7, 0x7FB9B9B9, 0x7FBBBBBB, 0x7FBDBDBD, 0x7FBFBFBF,
									0x7FC1C1C1, 0x7FC3C3C3, 0x7FC5C5C5, 0x7FC7C7C7, 0x7FC9C9C9, 0x7FCBCBCB, 0x7FCDCDCD, 0x7FCFCFCF, 0x7FD1D1D1, 0x7FD3D3D3, 0x7FD5D5D5, 0x7FD7D7D7, 0x7FD9D9D9, 0x7FDBDBDB, 0x7FDDDDDD, 0x7FDFDFDF,
									0x7FE1E1E1, 0x7FE3E3E3, 0x7FE5E5E5, 0x7FE7E7E7, 0x7FE9E9E9, 0x7FEBEBEB, 0x7FEDEDED, 0x7FEFEFEF, 0x7FF1F1F1, 0x7FF3F3F3, 0x7FF5F5F5, 0x7FF7F7F7, 0x7FF9F9F9, 0x7FFBFBFB, 0x7FFDFDFD, 0x7FFFFFFF };

#pragma endregion Palettes

bool isGem(const uint32_t &animSetId, const uint32_t &animId)
{
	return ((animSetId == ANIM_SET_ITEMS) && (animId == ANIM_GEM));
}

uint32_t htonl(uint32_t val)
{
	uint32_t out = 0;
	
	uint8_t *arr1 = (uint8_t*)&val;
	uint8_t *arr2 = (uint8_t*)&out;
	
	arr2[0] = arr1[3];
	arr2[1] = arr1[2];
	arr2[2] = arr1[1];
	arr2[3] = arr1[0];

	return out;
}

Animations::Animations(const char *filename)
{	
	FILE *fi = openFile(filename, "rb");
	if (fi == nullptr)
		return;
	ALIB_Header alib;
	ReadALIBHeader(fi, &alib);
	for (uint32_t i = 0; i < alib.SetCount; i++)
	{
		fseek(fi, alib.SetAddress[i], SEEK_SET);
		AnimationSet animSet(fi, i, ((i == 60) || (i == 61)) ? palette2 : palette1);
		animSets.push_back(animSet);
	}
	fclose(fi);
}

void Animations::DumpToDisk(const char *directory) const
{
	for (int i = 0; i < animSets.size(); i++)
	{
		for (int j = 0; j < animSets[i].GetAnimCount(); j++)
		{
			const Animation *anim = animSets[i].GetAnim(j);
			for (int k = 0; k < anim->GetFrameCount(); k++)
			{
				const AnimationFrame *frame = anim->GetFrame(k);
				if (frame != nullptr)
				{
					char buffer[256];
					sprintf_s(buffer, 255, "AnimSet%02d-Anim%02d-Frame%02d.bmp", i, j, k);
					frame->SaveBMP(Path::Combine(directory, buffer).c_str());
				}
			}
		}
	}
}

uint32_t Animations::GetAnimSetCount() const
{
	return animSets.size();
}

const AnimationSet *Animations::GetAnimSet(const uint32_t &index) const
{
	if(index < animSets.size())
	{
		return &animSets[index];
	}
	return nullptr;
}

Animations::~Animations()
{
}

AnimationSet::AnimationSet(FILE *fi, const uint32_t &animSetId, const uint32_t *palette)
{
	ReadANIMHeader(fi, &header);
	uint8_t *compressed = new uint8_t[header.CData1];
	fread(compressed, 1, header.CData1, fi);
	uint8_t *Data = new uint8_t[header.UData1];
	uLongf ucompSize = header.UData1;
	uncompress(Data, &ucompSize, compressed, header.CData1);
	delete[] compressed;

	std::vector<AnimInfo> animationInfos;
	for (int i = 0; i < header.AnimationCount; i++)
	{
		AnimInfo info;
		memcpy(&info, &Data[i*sizeof(AnimInfo)], sizeof(AnimInfo));
		animationInfos.push_back(info);
	}

	delete[] Data;
	compressed = new uint8_t[header.CData2];
	fread(compressed, 1, header.CData2, fi);
	Data = new uint8_t[header.UData2];
	ucompSize = header.UData2;
	uncompress(Data, &ucompSize, compressed, header.CData2);
	delete[] compressed;

	std::vector<FrameInfo> frameInfos;
	std::vector<uint32_t> animIds;
	int offset = 0;
	for (int i = 0; i < header.AnimationCount; i++)
	{
		for (int j = 0; j < animationInfos[i].FrameCount; j++)
		{
			FrameInfo info;
			memcpy(&info, &Data[(j*sizeof(FrameInfo)) + offset], sizeof(FrameInfo));
			frameInfos.push_back(info);
			animIds.push_back(i);
		}
		offset += 24 * animationInfos[i].FrameCount;
	}

	delete[] Data;
	compressed = new uint8_t[header.CData3];
	fread(compressed, 1, header.CData3, fi);
	Data = new uint8_t[header.UData3];
	ucompSize = header.UData3;
	uncompress(Data, &ucompSize, compressed, header.CData3);
	delete[] compressed;

	std::vector<FrameImage> frameImages;
	for (int i = 0; i < frameInfos.size(); i++)
	{
		FrameImage image(&Data[frameInfos[i].ImageAddress], (header.UData3 - frameInfos[i].ImageAddress), isGem(animSetId, animIds[i]) ? palette3 : palette);
		frameImages.push_back(image);
	}

	delete[] Data;
	compressed = new uint8_t[header.CData4];
	fread(compressed, 1, header.CData4, fi);
	Data = new uint8_t[header.UData4];
	ucompSize = header.UData4;
	uncompress(Data, &ucompSize, compressed, header.CData4);
	delete[] compressed;

	// Parse Data4

	delete[] Data;

	offset = 0;
	for (int i = 0; i < animationInfos.size(); i++)
	{
		vector<AnimationFrame> frames;
		for (int j = 0; j < animationInfos[i].FrameCount; j++)
		{
			frames.push_back(AnimationFrame(frameInfos[offset + j], frameImages[offset + j]));
		}
		offset += animationInfos[i].FrameCount;
		Anims.push_back(Animation(animationInfos[i], frames));
	}
}

uint32_t AnimationSet::GetAnimCount() const
{
	return Anims.size();
}

const Animation *AnimationSet::GetAnim(const uint32_t &index) const
{
	if (index < Anims.size())
	{
		return &Anims[index];
	}
	return nullptr;
}

AnimationSet::~AnimationSet()
{
}

FrameImage::FrameImage(uint8_t *bytes, const uint32_t length, const uint32_t *palette) : pixels(nullptr)
{
	int offset = 0;
	ReadAndAdvance(width, bytes, offset);
	ReadAndAdvance(height, bytes, offset);

	uint32_t alpha = (width & 0x8000) ? ((IS_BIG_ENDIAN) ? 0x0000007F : 0x7F000000 ) : ((IS_BIG_ENDIAN) ? 0x000000FF : 0xFF000000);
	width &= 0x7fff;

	clip = new uint8_t[width * height];
	pixels = new uint32_t[width * height];
	memset(clip, 0, width * height * sizeof(uint8_t));
	memset(pixels, 0, width*height*sizeof(uint32_t));
	int j = 0;
	while ((uint32_t(offset) < length) && (j < (width * height)))
	{
		uint8_t codebyte = 0;
		ReadAndAdvance(codebyte, bytes, offset);
		if (codebyte < 128)
		{
			for (int i = 0; i < codebyte; i++)
			{
				j++;
			}
		}
		else if (codebyte == 128)
		{
			do
			{
				j++;
			} while ((j%width) > 0);
		}
		else if (codebyte > 128)
		{
			for (int k = 0; k < (codebyte - 128); k++)
			{
				clip[j] = 0xFF;
				/*if (alpha & palette[bytes[offset + 1]])
					pixels[j++] = (palette[bytes[offset++]]);
				else
					*/pixels[j++] = (palette[bytes[offset++]] | alpha);
			}
			if ((j%width) == 0)
				j--;
		}
	}
}

FrameImage::FrameImage(const FrameImage &other) : pixels(nullptr)
{
	width = other.width;
	height = other.height;
	clip = new uint8_t[width * height];
	pixels = new uint32_t[width * height];
	memcpy(clip, other.clip, width * height * sizeof(uint8_t));
	memcpy(pixels, other.pixels, width * height * sizeof(uint32_t));
}

uint32_t *FrameImage::getPixels() const
{
	uint32_t *pix = new uint32_t[width * height];
	memcpy(pix, this->pixels, width * height * sizeof(uint32_t));
	return pix;
}

uint8_t *FrameImage::getClip() const
{
	uint8_t *c = new uint8_t[width * height];
	memcpy(c, clip, width * height * sizeof(uint8_t));
	return c;
}

void FrameImage::SaveBMP(const char *filename) const
{
	SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(pixels, width, height, 32, width * 4, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
	SDL_SaveBMP(surface, filename);
	SDL_FreeSurface(surface);
}

FrameImage::~FrameImage()
{
	if (clip != nullptr)
	{
		delete[] clip;
		clip = nullptr;
	}
	if (pixels != nullptr)
	{
		delete[] pixels;
		pixels = nullptr;
	}
}

AnimationFrame::AnimationFrame(FrameInfo info, FrameImage image): 
info(info), 
image(image),
frameIndex(FrameCount++)
{
}

uint32_t AnimationFrame::getWidth() const
{
	return info.Width;
}

uint32_t AnimationFrame::getHeight() const
{
	return info.Height;
}

uint32_t *AnimationFrame::getImage() const
{
	return image.getPixels();
}

uint8_t *AnimationFrame::getClip() const
{
	return image.getClip();
}

vec2 AnimationFrame::getHotSpot() const
{
	return vec2(info.HotspotX, info.HotspotY);
}

vec2 AnimationFrame::getColdSpot() const
{
	return vec2(info.ColdspotX, info.ColdspotY);
}

vec2 AnimationFrame::getGunSpot() const
{
	return vec2(info.GunspotX, info.GunspotY);
}

AnimationFrame::~AnimationFrame()
{

}

Animation::Animation(AnimInfo info, vector<AnimationFrame> frames) :
info(info), 
frames(frames)
{
}

uint32_t Animation::GetFrameCount() const
{
	return frames.size();
}

uint32_t Animation::GetFrameRate() const
{
	return (uint32_t)Math::Round(info.FrameRate * 1.0f);
}

const AnimationFrame *Animation::GetFrame(const uint32_t &index) const
{
	if (index < frames.size())
	{
		return &frames[index];
	}
	return nullptr;
}

uint32_t AnimationFrame::getIndex() const
{
	return frameIndex;
}

void AnimationFrame::SaveBMP(const char *filename) const
{
	image.SaveBMP(filename);
}

Animation::~Animation()
{

}
