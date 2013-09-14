<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>Label</key>
		<string>BUNDLE_ID_PREFIX.privileged_startx</string>
	<key>ProgramArguments</key>
		<array>
			<string>XINITDIR/privileged_startx</string>
			<string>-d</string>
			<string>SCRIPTDIR</string>
		</array>
#ifdef TIGER_LAUNCHD
	<key>RunAtLoad</key>
		<true/>
	<key>KeepAlive</key>
		<true/>
	<key>ServiceIPC</key>
		<true/>
#else
	<key>MachServices</key>
		<dict>
			<key>BUNDLE_ID_PREFIX.privileged_startx</key>
				<true/>
		</dict>
	<key>TimeOut</key>
		<integer>120</integer>
	<key>EnableTransactions</key>
		<true/>
#endif
</dict>
</plist>
