################################################################################
#
# spiral_sound
#
################################################################################

SPIRAL_SOUND_VERSION = 1.0
SPIRAL_SOUND_SITE = /home/andyp/Documents/projects/crystal-palace/spiralSound
SPIRAL_SOUND_SITE_METHOD = local
SPIRAL_SOUND_DEPENDENCIES = libao alsa-lib

define SPIRAL_SOUND_BUILD_CMDS
	$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(@D)/demo
endef

define SPIRAL_SOUND_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/demo/demosynth $(TARGET_DIR)/usr/bin/demosynth
endef

$(eval $(generic-package))
