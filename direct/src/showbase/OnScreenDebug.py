

from PandaModules import *

import OnscreenText
import DirectUtil

class OnScreenDebug:
    def __init__(self):
        fontPath = config.GetString("on-screen-debug-font", "cmtt12")
        fontScale = config.GetFloat("on-screen-debug-font-scale", 0.05)
        self.enabled = config.GetBool("on-screen-debug-enabled", 0)
        font = loader.loadFont(fontPath)
        if not font.isValid():
            print "failed to load OnScreenDebug font", fontPath
            font = TextNode.getDefaultFont()
        self.onScreenText = OnscreenText.OnscreenText(
                pos = (-1.0, 0.9), bg=Vec4(1,1,1,0.85),
                scale = (fontScale, fontScale, 0.0), align = TextNode.ALeft,
                mayChange = 1, font = font)
        # Make sure readout is never lit or drawn in wireframe
        DirectUtil.useDirectRenderStyle(self.onScreenText)
        self.frame = 0
        self.text = ""
        self.data = {}

    def render(self):
        if not self.enabled:
            return
        self.onScreenText.clearText()
        for k, v in self.data.items():
            if v[0] == self.frame:
                # It was updated this frame (key equals value):
                #isNew = " is"
                isNew = "="
            else:
                # This data is not for the current 
                # frame (key roughly equals value):
                #isNew = "was"
                isNew = "~"
            self.onScreenText.appendText("%20s %s %-44s\n"%(k, isNew, v[1]))
        self.onScreenText.appendText(self.text)
        self.frame += 1

    def clear(self):
        self.text = ""
        self.onScreenText.clearText()

    def add(self, key, value):
        self.data[key] = (self.frame, value)
        return 1 # to allow assert(onScreenDebug.add("foo", bar))

    def remove(self, key):
        del self.data[key]

    def append(self, text):
        self.text += text
