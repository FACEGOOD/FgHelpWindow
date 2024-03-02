/*
 * Copyright (c) 2023-2033 FACEGOOD, Inc. All Rights Reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License V3 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * V3 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
/** @file FgEventTracking.cpp
 *  @brief 
 *
 *  @author Nero
 */

#include "FgEventTracking.h"
#include <json.hpp>
#include <FgHttps.h>
#include <string>
#include <Windows.h>
#include <clocale>

#pragma comment(lib, "advapi32.lib")


std::string FgEventTracking::mAppName="";

HKEY getHKEY(int nKeyType) {
    HKEY hKey = NULL;
    switch (nKeyType) {
    case 0: {
        hKey = HKEY_CLASSES_ROOT;
        break;
    }
    case 1: {
        hKey = HKEY_CURRENT_USER;
        break;
    }
    case 2: {
        hKey = HKEY_LOCAL_MACHINE;
        break;
    }
    case 3: {
        hKey = HKEY_USERS;
        break;
    }
    case 4: {
        hKey = HKEY_PERFORMANCE_DATA;
        break;
    }
    case 5: {
        hKey = HKEY_CURRENT_CONFIG;
        break;
    }
    case 6: {
        hKey = HKEY_DYN_DATA;
        break;
    }
    case 7: {
        hKey = HKEY_CURRENT_USER_LOCAL_SETTINGS;
        break;
    }
    case 8: {
        hKey = HKEY_PERFORMANCE_TEXT;
        break;
    }
    case 9: {
        hKey = HKEY_PERFORMANCE_NLSTEXT;
        break;
    }
    }
    return hKey;
}

std::string GetRegValue(int hKeyType, const std::string& strUrl,
    const std::string& strKey) {
    std::string strValue("");
    HKEY hKey = NULL;
    HKEY hKeyResult = NULL;
    DWORD dwSize = 0;
    DWORD dwDataType = 0;

    hKey = getHKEY(hKeyType);

    if (ERROR_SUCCESS ==
        ::RegOpenKeyExA(hKey, strUrl.c_str(), 0, KEY_QUERY_VALUE, &hKeyResult)) {
        ::RegQueryValueExA(hKeyResult, strKey.c_str(), 0, &dwDataType, NULL,
            &dwSize);
        switch (dwDataType) {
        case REG_MULTI_SZ: {
            //�����ڴ��С
            BYTE* lpValue = new BYTE[dwSize];
            LONG lRet = ::RegQueryValueExA(hKeyResult, strKey.c_str(), 0,
                &dwDataType, lpValue, &dwSize);
            delete[] lpValue;
            break;
        }
        case REG_SZ: {
            char* lpValue = new char[dwSize];
            memset(lpValue, 0, dwSize * sizeof(char));
            if (ERROR_SUCCESS == ::RegQueryValueExA(hKeyResult, strKey.c_str(), 0,
                &dwDataType, (LPBYTE)lpValue,
                &dwSize)) {
                strValue = std::string(lpValue);
            }
            delete[] lpValue;
            break;
        }
        case REG_DWORD: {
            DWORD data = 0;                 // to be read from the registry
            DWORD dataSize = sizeof(data);  // size of data, in bytes

            constexpr DWORD flags = RRF_RT_REG_DWORD;
            LONG retCode = RegGetValueA(hKeyResult,
                nullptr,  // no subkey
                strKey.c_str(), flags,
                nullptr,  // type not required
                &data, &dataSize);
            if (retCode != ERROR_SUCCESS) {
                strValue = "";
            }

            strValue = std::to_string(data);
        }
        default:
            break;
        }
    }

    //�ر�ע���
    ::RegCloseKey(hKeyResult);

    return strValue;
}


bool FgEventTracking::init(const std::string& appName)
{
    FgHttps::Init(GetRegValue(1, "Software\\FACEGOOD\\Avatary\\AppInfo", "SoftVersion"));

    nlohmann::json j;
    std::string url = "/v1/member/info";
    nlohmann::json response;

    try {
        response = FgHttps::Get(url, j);
    }
    catch (const std::exception& e) {
        ;
    }

    if (response["code"] != 200)
    {
        ;
    }

    std::string userId = response["data"]["memberNo"].get<std::string>();


    if (appName == "AutofaceHelp.json")
    {
        FgCaptureDataO::Init(fgdc::APPTYPE::AUTOFACE);
        mAppName = "AutoFace";
    }
    else if (appName == "LiveDriveHelp.json") {
        FgCaptureDataO::Init(fgdc::APPTYPE::LIVEDRIVE);
        mAppName = "LiveDrive";

    }
    else if (appName == "StreamerHelp.json") {
        FgCaptureDataO::Init(fgdc::APPTYPE::STREAMER);
        mAppName = "Streamer";

    }
    else if (appName == "TrackerHelp.json") {
        FgCaptureDataO::Init(fgdc::APPTYPE::TRACKER);
        mAppName = "Tracker";

    }

	FgCaptureDataO::setHeader(userId);
    return false;
}

//bool FgEventTracking::Event_ClickNewProjectBtn(const std::string& templateName)
//{
//	nlohmann::json body;
//	body["templateName"] = templateName;
//	FgCaptureDataO::setBody("Click_TrackerPushBtu1", body);
//	FgCaptureDataO::commit();
//	return false;
//}
//
//bool FgEventTracking::Event_ClickNewCustomTemplate(const std::string& customTemplateName)
//{
//	nlohmann::json body;
//	body["customTemplateName"] = customTemplateName;
//	FgCaptureDataO::setBody("Click_TrackerPushBtu1", body);
//	FgCaptureDataO::commit();
//	return false;
//}
//
//bool FgEventTracking::Event_ClickBuildBtn(int pointCount)
//{
//	nlohmann::json body;
//	body["pointCount"] = pointCount;
//	FgCaptureDataO::setBody("Click_TrackerPushBtu1", body);
//	FgCaptureDataO::commit();
//	return false;
//}

bool FgEventTracking::Event_ReadDocmentTab(int duration)
{
    nlohmann::json body;
    body["appName"] = mAppName;
    body["browserTime"] = duration;
    FgCaptureDataO::setBody("ReadDocmentTab", body);
    FgCaptureDataO::commit();
    return false;
}

bool FgEventTracking::Event_ReadFAQTab(int duration)
{
    nlohmann::json body;
    body["appName"] = mAppName;
    body["browserTime"] = duration;
    FgCaptureDataO::setBody("ReadFAQTab", body);
    FgCaptureDataO::commit();
    return false;
}

bool FgEventTracking::Event_ReadShortCutTab(int duration)
{
    nlohmann::json body;
    body["appName"] = mAppName;
    body["browserTime"] = duration;
    FgCaptureDataO::setBody("ReadShortCutTab", body);
    FgCaptureDataO::commit();
	return false;
}

bool FgEventTracking::storage()
{
	FgCaptureDataO::storage();
	return false;
}

