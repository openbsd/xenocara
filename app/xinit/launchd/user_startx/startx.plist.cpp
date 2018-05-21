<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>Label</key>
		<string>BUNDLE_ID_PREFIX.startx</string>
	<key>ProgramArguments</key>
		<array>
			<string>__libexecdir__/launchd_startx</string>
			<string>__bindir__/startx</string>
#ifdef LAUNCHAGENT_XSERVER_PATH
			<string>--</string>
			<string>LAUNCHAGENT_XSERVER_PATH</string>
#endif
		</array>
	<key>Sockets</key>
		<dict>
			<key>BUNDLE_ID_PREFIX:0</key>
				<dict>
					<key>SecureSocketWithKey</key>
						<string>DISPLAY</string>
				</dict>
		</dict>
	<key>ServiceIPC</key>
		<true/>
	<key>EnableTransactions</key>
		<true/>
</dict>
</plist>
