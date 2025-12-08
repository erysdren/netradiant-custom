/*
   Copyright (C) 2001-2006, William Joseph.
   All Rights Reserved.

   This file is part of GtkRadiant.

   GtkRadiant is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   GtkRadiant is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GtkRadiant; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#pragma once

/// \file
/// \brief OS directory-listing object.

#include <filesystem>
#include <concepts>
#include <functional>

void Directory_forEach(char const* path, std::invocable<char const*> auto const& functor) {
	std::error_code error { };
	// will construct and check, and also do additional error checks like permissions
	std::filesystem::directory_iterator directory { path, error };
	if (error) { /* handle error here, or just return */ return; }
	for (auto&& entry : directory) {
		// On Windows, .filename().c_str() will return a wchar_t*,
		// not a char const*, which is what the glib libraries do.
		// This also keeps the name with a valid forward slash
		std::invoke(functor, reinterpret_cast<char const*>(entry.path().filename().generic_u8string().c_str()));
	}
}
