import platform
from time import sleep

import pexpect as pexpect
from mbed_host_tests import BaseHostTest, event_callback


class DebugTests(BaseHostTest):
    """
    """

    def __init__(self):
        # generate key pair (server side)
        system = platform.system()
        if system == 'Linux':
            self.jlink = 'JLinkExe'
        elif system == 'Windows':
            self.link = 'JLink.exe'
        elif system == 'Darwin':
            self.jlink = 'JLinkExe'

        self.log("JLink: found: " + self.jlink)
        if (self.jlink is None):
            raise EnvironmentError("JLink executable not found")

        BaseHostTest.__init__(self)

    def swoReady(self):
        swo = None
        while swo is None:
            swo = pexpect.spawn("JLinkExe -device NRF52 -if SWD -speed auto -autoconnect 1")
            swo.sendline("swoview 4000000")
            swo.expect(["FAILED", "-----------------------------------------------"])
            if swo.match.group(0) == "FAILED":
                self.log("J-Link busy, retry ...")
                swo.sendcontrol('C')
                while swo.isalive():
                    sleep(0.1)
                swo = None
                sleep(4)
        self.send_kv("swo-ready", "ok")
        return swo

    @event_callback("expect")
    def __expect(self, key, value, timestamp):
        swo = None
        try:
            swo = self.swoReady()
            swo.expect(value)
            if len(swo.match.groups()) == 0:
                raise Exception("NOT FOUND")
            for group in swo.match.groups():
                self.send_kv("received", group)
        except Exception as e:
            self.send_kv("error", e.message)
        finally:
            if swo is not None:
                swo.sendcontrol('C')
                while swo.isalive():
                    sleep(0.1)
