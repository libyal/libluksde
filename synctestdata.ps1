# Script that synchronizes the local test data
#
# Version: 20230709

$TestSet = "public"
$TestInputDirectory = "tests/input"
$TestFiles = "luks1.raw"

If (-Not (Test-Path ${TestInputDirectory}))
{
	New-Item -Name ${TestInputDirectory} -ItemType "directory" | Out-Null
}
If (-Not (Test-Path "${TestInputDirectory}\${TestSet}"))
{
	New-Item -Name "${TestInputDirectory}\${TestSet}" -ItemType "directory" | Out-Null
	New-Item -Name "${TestInputDirectory}\.libluksde\${TestSet}" -ItemType "directory" | Out-Null
	New-Item -Name "${TestInputDirectory}\.pyluksde\${TestSet}" -ItemType "directory" | Out-Null
	New-Item -Name "${TestInputDirectory}\.luksdeinfo\${TestSet}" -ItemType "directory" | Out-Null
}

@"
# libyal test data options
password=luksde-TEST
"@ | Out-File -FilePath test_data_options

ForEach ($TestFile in ${TestFiles} -split " ")
{
	$Url = "https://github.com/log2timeline/dfvfs/blob/main/test_data/${TestFile}?raw=true"

	Invoke-WebRequest -Uri ${Url} -OutFile "${TestInputDirectory}\${TestSet}\${TestFile}"

	Copy-Item test_data_options -Destination "${TestInputDirectory}\.libluksde\${TestSet}\${TestFile}.password"
	Copy-Item test_data_options -Destination "${TestInputDirectory}\.pyluksde\${TestSet}\${TestFile}.password"
	Copy-Item test_data_options -Destination "${TestInputDirectory}\.luksdeinfo\${TestSet}\${TestFile}.password"
}

