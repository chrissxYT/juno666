/*
 * Copyright(c) 2000 UltraMaster Group
 *
 * License to use UltraMaster Juno-6 is provided free of charge subject to the
 * following restrictions:
 *
 * 1.) This license is for your personal use only.
 *
 * 2.) No portion of this software may be redistributed by you to any other
 *     person in any form.
 *
 * 3.) You may not sell UltraMaster Juno-6 to any person.
 *
 * 4.) UltraMaster Juno-6 is provided without any express or implied warranty.
 *     In no event shall UltraMaster Group be held liable for any damages
 *     arising from the use of UltraMaster Juno-6.
 */
/**
 * Copyright (c) UltraMaster Group, LLC. All Rights Reserved.
 * $Revision: 1.3 $$Date: 2004/03/31 12:01:19 $
 */
#ifndef SETTINGS_H
#define SETTINGS_H

#include <libmoogutil/ini.h>
#include <libmoogutil/String.h>

class Settings
{
private:
	struct ini_file *iniFile;

public:
	Settings();
	~Settings();
	int getInt(const char *sectionName, const char *itemName);
	const char *getString(const char *sectionName, const char *itemName);
};

#endif /* SETTINGS_H */

