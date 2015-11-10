#!/usr/bin/env python
#
# Copyright 2015-present Facebook. All Rights Reserved.
#
# This program file is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; version 2 of the License.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program in a file named COPYING; if not, write to the
# Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301 USA
#


from subprocess import *
import re
from node import node
from pal import *

class bmcNode(node):
    def __init__(self, info = None, actions = None):
        if info == None:
            self.info = {}
        else:
            self.info = info
        if actions == None:
            self.actions = []
        else:
            self.actions = actions

    def getInformation(self):
        # Get Platform Name
        name = pal_get_platform_name()

        # Get BMC Reset Reason
        wdt_counter = Popen('devmem 0x1e785010', \
                            shell=True, stdout=PIPE).stdout.read()
        wdt_counter = int(wdt_counter, 0)

        wdt_counter &= 0xff00

        if wdt_counter:
            por_flag = 0
        else:
            por_flag = 1

        if por_flag:
            reset_reason = "Power ON Reset"
        else:
            reset_reason = "User Initiated Reset or WDT Reset"

        # Get BMC's Up Time
        uptime = Popen('uptime', \
                        shell=True, stdout=PIPE).stdout.read()

        # Get Usage information
        data = Popen('top -b n1', \
                            shell=True, stdout=PIPE).stdout.read()
        adata = data.split('\n')
        mem_usage = adata[0]
        cpu_usage = adata[1]

        # Get OpenBMC version
        version = ""
        data = Popen('cat /etc/issue', \
                            shell=True, stdout=PIPE).stdout.read()
        #Version might start with 'v'(wedge) or 'V'(Yosemite)
        if name == 'Yosemite':
            ver = re.search(r'V([\w\d._-]*)\s', data)
        else:
            ver = re.search(r'v([\w\d._-]*)\s', data)
        if ver:
            version = ver.group(1)


        info = {
            "Description": name + " BMC",
            "Reset Reason": reset_reason,
            "Uptime": uptime,
            "Memory Usage": mem_usage,
            "CPU Usage": cpu_usage,
            "OpenBMC Version": version,
            }

        return info;

def get_node_bmc():
    return bmcNode()
