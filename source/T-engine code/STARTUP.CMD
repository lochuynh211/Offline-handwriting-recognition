*
*	@(#)STARTUP.CMD (T-Engine/SH7760)
*
*	IMS initial command script for booting system
*	Copyright (C) 2004 by Personal Media Corporation
*
$chgsys	uda rda
lodspg	kbpd		!30
lodspg	lowkbpd		!28
lodspg	rsdrv		!26
lodspg	screen		!35
lodspg	scrtst
lodspg	unixemu
lodspg recog
$cli	STARTUP.CLI	!224
exit	1
