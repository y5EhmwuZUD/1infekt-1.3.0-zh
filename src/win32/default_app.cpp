/**
* Copyright (C) 2016 syndicode
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
**/

#include "stdafx.h"
#include "default_app.h"
#include "util.h"

#define DEFAULT_APP_REG_NAME L"iNFekt NFO Viewer" // Name in HKLM\SOFTWARE\RegisteredApplications
#define DEFAULT_APP_PROG_ID L"iNFEKT.NFOFile.1"
#define DEFAULT_APP_EXTENSION L".nfo"

std::unique_ptr<CWinDefaultApp> CWinDefaultApp::Factory()
{
	return std::make_unique<CWin8DefaultApp>(DEFAULT_APP_PROG_ID, DEFAULT_APP_EXTENSION);
}

void CWinDefaultApp::CheckDefaultNfoViewer(HWND a_hwnd)
{
	if (!CanCheckDefaultNfoViewer())
	{
		return;
	}
	else if (this->IsDefault())
	{
		return;
	}

	if (::MessageBoxW(a_hwnd, L"iNFekt 不是默认的 NFO 查看器。是否立即将其设为默认查看器？",
		L"重要提示", MB_ICONQUESTION | MB_YESNO) == IDYES)
	{
		if (this->MakeDefault() == MakeDefaultResult::FAILED)
		{
			::MessageBoxW(a_hwnd, L"将 iNFekt 设为默认 NFO 查看器时出现问题，请手动完成设置。", L"问题", MB_ICONEXCLAMATION);
		}
	}
}

void CWinDefaultApp::CheckDefaultNfoViewerInteractive(HWND a_hwnd)
{
	if (!CanCheckDefaultNfoViewer())
	{
		::MessageBoxW(a_hwnd, L"iNFekt 未正确安装，或者您正在使用便携版。"
			L"很遗憾，iNFekt 无法自动修复此问题。请使用安装程序重新安装 iNFekt，或手动"
			L"将 iNFekt 与 .nfo 文件关联。", L"问题", MB_ICONEXCLAMATION);
	}
	else if (this->IsDefault())
	{
		::MessageBoxW(a_hwnd, L"iNFekt 当前似乎已经是默认 NFO 查看器！", L"操作成功", MB_ICONINFORMATION);
	}
	else if (::MessageBoxW(a_hwnd, L"iNFekt 不是默认的 NFO 查看器。是否立即将其设为默认查看器？",
		L"重要提示", MB_ICONQUESTION | MB_YESNO) == IDYES)
	{
		MakeDefaultResult result = this->MakeDefault();

		if (result == MakeDefaultResult::SUCCEEDED)
		{
			// ensure consistent behaviour:
			if (this->IsDefault())
			{
				::MessageBoxW(a_hwnd, L"iNFekt 现在已是默认 NFO 查看器！", L"操作成功", MB_ICONINFORMATION);
			}
			else
			{
				result = MakeDefaultResult::FAILED;
			}
		}

		if (result == MakeDefaultResult::FAILED)
		{
			::MessageBoxW(a_hwnd, L"将 iNFekt 设为默认 NFO 查看器时出现问题，请手动完成设置。", L"问题", MB_ICONEXCLAMATION);
		}
	}
}

bool CWinDefaultApp::CanCheckDefaultNfoViewer()
{
	if (!this->IsDefault() && this->GotNoSuchProgramName())
	{
		return false;
	}

	return true;
}
