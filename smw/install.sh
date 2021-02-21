#!/bin/bash
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Fewtarius
# Coypright (C) 2021-present Cebion

INSTALLPATH="/roms/ports"
PKG_NAME="smw"
PKG_FILE="smw-gamedata.zip"
PKG_VERSION="1.7"
PKG_SHASUM="e679598294f5aa531cc88cfb8fa3034e100c35108ebbacf00ae0b4b2306ea0e7"
SOURCEPATH=$(pwd)
### Test and make the full path if necessary.
if [ ! -d "${INSTALLPATH}/${PKG_NAME}" ]
then
  mkdir -p "${INSTALLPATH}/${PKG_NAME}"
fi

cd ${INSTALLPATH}/${PKG_NAME}

curl -Lo ${PKG_FILE} https://github.com/Cebion/packages/releases/download/${PKG_VERSION}/${PKG_FILE}
BINSUM=$(sha256sum ${PKG_FILE} | awk '{print $1}')
if [ ! "${PKG_SHASUM}" == "${BINSUM}" ]
then
  echo "Checksum mismatch, please update the package."
  exit 1
fi

unzip -o ${PKG_FILE}
rm ${PKG_FILE}
cp -r ./.smw/ ~/

### Create the start script
cat <<EOF >${INSTALLPATH}/"Smw.sh"
#!/bin/bash
source /etc/profile
/usr/bin/show_splash.sh "Super Mario War"
clear >/dev/console

cd ${INSTALLPATH}/${PKG_NAME}
./smw

EOF

### Add images if not exist folder
if [ ! -d "${INSTALLPATH}/images" ]
then
  mkdir -p "${INSTALLPATH}/images"
fi

### Add videos if not exist folder
if [ ! -d "${INSTALLPATH}/videos" ]
then
  mkdir -p "${INSTALLPATH}/videos"
fi

for image in system-smw-thumb.png system-smw.png
do
  cp "${SOURCEPATH}/${PKG_NAME}/${image}" "${INSTALLPATH}/images"
done

for video in system-smw.mp4
do
  cp "${SOURCEPATH}/${PKG_NAME}/${video}" "${INSTALLPATH}/videos"
done

### Add Game to the game list XML
if [ ! "$(grep 'Super Mario War' ${INSTALLPATH}/gamelist.xml)" ]
then
	### Add to the game list
	xmlstarlet ed --omit-decl --inplace \
		-s '//gameList' -t elem -n 'game' \
		-s '//gameList/game[last()]' -t elem -n 'path'        -v './Smw.sh'\
		-s '//gameList/game[last()]' -t elem -n 'name'        -v 'Super Mario War'\
		-s '//gameList/game[last()]' -t elem -n 'desc'        -v 'Super Mario War is a multiplayer game. The goal is to stomp as many other Marios as possible to win the game.'\
		-s '//gameList/game[last()]' -t elem -n 'genre'       -v 'Action'\
		-s '//gameList/game[last()]' -t elem -n 'image'       -v './images/system-smw.png'\
		-s '//gameList/game[last()]' -t elem -n 'thumbnail'   -v './images/system-smw-thumb.png'\
                -s '//gameList/game[last()]' -t elem -n 'video'       -v './videos/system-smw.mp4'\
		-s '//gameList/game[last()]' -t elem -n 'rating'      -v '4.0'\
		-s '//gameList/game[last()]' -t elem -n 'releasedate' -v '2005'\
		-s '//gameList/game[last()]' -t elem -n 'developer'   -v 'Michael Schaffer, Florian Hufsky'\
		-s '//gameList/game[last()]' -t elem -n 'publisher'   -v 'OpenSource GPL'\
		${INSTALLPATH}/gamelist.xml
fi
