#!/bin/bash
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Fewtarius
# Coypright (C) 2020-present Cebion

INSTALLPATH="/storage/roms/ports"
PKG_NAME="iortcw"
PKG_FILE="arkos-iortcw_f7b50105.zip"
PKG_VERSION="0.2-beta"
PKG_SHASUM="fd3c56c3769593c6ba8617321e7c60d5a6f8ebe4a80ec65379f240fc45c09cb2"
SOURCEPATH=$(pwd)
### Test and make the full path if necessary.
if [ ! -d "${INSTALLPATH}/${PKG_NAME}" ]
then
  mkdir -p "${INSTALLPATH}/${PKG_NAME}"
fi

cd ${INSTALLPATH}

curl -Lo ${PKG_FILE} https://github.com/JohnnyonFlame/handheld-iortcw/releases/download/${PKG_VERSION}/${PKG_FILE}
BINSUM=$(sha256sum ${PKG_FILE} | awk '{print $1}')
if [ ! "${PKG_SHASUM}" == "${BINSUM}" ]
then
  echo "Checksum mismatch, please update the package."
  exit 1
fi

unzip -o ${PKG_FILE}
rm ${PKG_FILE}
rm 'Return to Castle Wolfenstein.sh'

### Create the start script
cat <<EOF >${INSTALLPATH}/"Iortcw.sh"
#!/bin/bash
source /etc/profile
/usr/bin/show_splash.sh "Return to Castle Wolfenstein"
clear >/dev/console
export SDL_GAMECONTROLLERCONFIG="03000000091200000031000011010000,OpenSimHardware OSH PB Controller,a:b0,b:b1,x:b2,y:b3,leftshoulder:b4,rightshoulder:b5,dpdown:h0.4,dpleft:h0.8,dpright:h0.2,dpup:h0.1,leftx:a0~,lefty:a1~,guide:b12,leftstick:b8,lefttrigger:b10,rightstick:b9,back:b7,start:b6,rightx:a2,righty:a3,righttrigger:b11,platform:Linux,"

/storage/roms/ports/iortcw/iowolfsp.aarch64

EOF

### Add images if not exist folder
if [ ! -d "${INSTALLPATH}/images" ]
then
  mkdir -p "${INSTALLPATH}/images"
fi

for image in system-iortcw.png  system-iortcw-thumb.png
do
  cp "${SOURCEPATH}/${PKG_NAME}/${image}" "${INSTALLPATH}/images"
done

### Add Iortcw to the game list
if [ ! "$(grep -q 'Return to Castle Wolfenstein' ${INSTALLPATH}/gamelist.xml)" ]
then
	### Add to the game list
	xmlstarlet ed --omit-decl --inplace \
		-s '//gameList' -t elem -n 'game' \
		-s '//gameList/game[last()]' -t elem -n 'path'        -v './Iortcw.sh'\
		-s '//gameList/game[last()]' -t elem -n 'name'        -v 'Return to Castle Wolfenstein'\
		-s '//gameList/game[last()]' -t elem -n 'desc'        -v 'Return to Castle Wolfenstein is a first-person shooter video game published by Activision.'\
		-s '//gameList/game[last()]' -t elem -n 'image'       -v './images/system-iortcw.png'\
		-s '//gameList/game[last()]' -t elem -n 'thumbnail'   -v './images/system-iortcw-thumb.png'\
                -s '//gameList/game[last()]' -t elem -n 'video'       -v './videos/system-iortcw.mp4'\
		-s '//gameList/game[last()]' -t elem -n 'rating'      -v '4.0'\
		-s '//gameList/game[last()]' -t elem -n 'releasedate' -v '2001'\
		-s '//gameList/game[last()]' -t elem -n 'developer'   -v 'Gray Matter Interactive'\
		-s '//gameList/game[last()]' -t elem -n 'publisher'   -v 'Activision'\
		${INSTALLPATH}/gamelist.xml
fi
