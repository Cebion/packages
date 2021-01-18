#!/bin/bash
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Fewtarius

INSTALLPATH="/storage/roms/ports"
PKG_NAME="iortcw"

rm -rf "${INSTALLPATH}/${PKG_NAME}"
rm -f "${INSTALLPATH}/Iortcw.sh"

for image in system-iortcw.png  system-iortcw-thumb.png
do
  rm "${INSTALLPATH}/images/${image}"
done
