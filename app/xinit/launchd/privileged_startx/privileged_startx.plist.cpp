<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>Label</key>
		<string>BUNDLE_ID_PREFIX.privileged_startx</string>
	<key>ProgramArguments</key>
		<array>
			<string>__libexecdir__/privileged_startx</string>
			<string>-d</string>
			<string>SCRIPTDIR</string>
		</array>
	<key>MachServices</key>
		<dict>
			<key>BUNDLE_ID_PREFIX.privileged_startx</key>
				<true/>
		</dict>
	<key>TimeOut</key>
		<integer>120</integer>
	<key>EnableTransactions</key>
		<true/>
</dict>
</plist>
