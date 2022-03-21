/*
 * SPDX-FileCopyrightText: (C) 2021-2022 Matthias Fehring / www.huessenbergnetz.de
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef SCHAUER_EXPORTS_H
#define SCHAUER_EXPORTS_H

#include <QtCore/QtGlobal>

#if defined(SchauerQt6_EXPORTS) || defined(SchauerQt5_EXPORTS)
#  define SCHAUER_LIBRARY Q_DECL_EXPORT
#else
#  define SCHAUER_LIBRARY Q_DECL_IMPORT
#endif

#endif // SCHAUER_EXPORTS_H
