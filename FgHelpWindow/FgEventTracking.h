/*
 * Copyright (C) 2023-2033 FACEGOOD, Inc. All Rights Reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
/** @file FgEventTracking.h
 *  @brief 
 *
 *  @author Nero
 */
#pragma once


#include <FgCaptureDataO.h>

class FgEventTracking
{
public:
	static bool init(const std::string& appName);
	//static bool Event_ClickNewProjectBtn(const std::string& templateName);
	//static bool Event_ClickNewCustomTemplate(const std::string& customTemplateName);
	//static bool Event_ClickBuildBtn(int pointCount);
	static bool Event_ReadDocmentTab(int duration);
	static bool Event_ReadFAQTab(int duration);
	static bool Event_ReadShortCutTab(int duration);

	static bool storage();

private:
	static std::string mAppName;
};

