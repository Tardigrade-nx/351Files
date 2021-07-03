#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (C) 2020-present Shanti Gilbert (https://github.com/shantigilbert)

# Source predefined functions and variables
. /etc/profile

PORT="351Files"
# init_port binary audio(alsa. pulseaudio, default)
init_port ${PORT} alsa

#~ /usr/bin/show_splash.sh "${PORT}"

clear >/dev/console

jslisten set "/usr/bin/killall ${PORT}"

cd /storage/roms/ports/"${PORT}"

./${PORT} &>>/tmp/logs/exec.log

jslisten stop

ret_error=$?

end_port

exit $ret_error
