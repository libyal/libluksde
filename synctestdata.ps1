# Script that synchronizes the local test data

$TestsInputDirectory = "tests\input"
$TestSet = "public"
$TestFiles = "luks1.raw"

If (-Not (Test-Path ${TestsInputDirectory}))
{
	New-Item -Name ${TestsInputDirectory} -ItemType "directory" | Out-Null
}
If (-Not (Test-Path "${TestsInputDirectory}\${TestSet}"))
{
	New-Item -Name "${TestsInputDirectory}\${TestSet}" -ItemType "directory" | Out-Null
}
ForEach ($TestFile in ${TestFiles} -split " ")
{
	$UrlTestFile = [System.Uri]::EscapeDataString("${TestFile}")
	$Url = "https://raw.githubusercontent.com/log2timeline/dfvfs/refs/heads/main/test_data/${UrlTestFile}"

	$ProgressPreference = 'SilentlyContinue'
	Invoke-WebRequest -Uri ${Url} -OutFile "${TestsInputDirectory}\${TestSet}\${TestFile}"
}

@"
# libyal test data options
password=luksde-TEST
"@ | Out-File -Encoding ascii -FilePath test_data_options

New-Item -Name "${TestsInputDirectory}\.libluksde\${TestSet}" -ItemType "directory" | Out-Null
Copy-Item test_data_options -Destination "${TestsInputDirectory}\.libluksde\${TestSet}\${TestFile}.password"

New-Item -Name "${TestsInputDirectory}\.pyluksde\${TestSet}" -ItemType "directory" | Out-Null
Copy-Item test_data_options -Destination "${TestsInputDirectory}\.pyluksde\${TestSet}\${TestFile}.password"

New-Item -Name "${TestsInputDirectory}\.luksdeinfo\${TestSet}" -ItemType "directory" | Out-Null
Copy-Item test_data_options -Destination "${TestsInputDirectory}\.luksdeinfo\${TestSet}\${TestFile}.password"

Remove-Item -Force -Path test_data_options
