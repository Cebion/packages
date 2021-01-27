#!/bin/bash
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Fewtarius

INSTALLPATH="/storage/roms/ports"
PKG_NAME="cdogs-sdl"

rm -rf "${INSTALLPATH}/${PKG_NAME}"
rm -f "${INSTALLPATH}/Cdogs-sdl.sh"

for image in system-cdogs.png  system-cdogs-thumb.png
do
  rm "${INSTALLPATH}/images/${image}"
done

for video in system-cdogs.mp4
do
  rm "${INSTALLPATH}/videos/${video}"
done

