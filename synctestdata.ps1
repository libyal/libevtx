# Script that synchronizes the local test data
#
# Version: 20260608

$Repository = "log2timeline/plaso"
$TestDataPath = "test_data/evtx"
$TestSet = "public"
$TestInputDirectory = "tests\input"
$TestFiles = "System.evtx"

If (-Not (Test-Path ${TestInputDirectory}))
{
	New-Item -Name ${TestInputDirectory} -ItemType "directory" | Out-Null
}
If (-Not (Test-Path "${TestInputDirectory}\.evtxexport_xml"))
{
	New-Item -Name "${TestInputDirectory}\.evtxexport_xml" -ItemType "directory" | Out-Null
	Write-Output "-fxml" | Out-File -Encoding ascii -FilePath "${TestInputDirectory}\.evtxexport_xml\options"
}
If (-Not (Test-Path "${TestInputDirectory}\${TestSet}"))
{
	New-Item -Name "${TestInputDirectory}\${TestSet}" -ItemType "directory" | Out-Null
}
ForEach ($TestFile in ${TestFiles} -split " ")
{
	$UrlTestFile = [System.Uri]::EscapeDataString("${TestFile}")
	$Url = "https://raw.githubusercontent.com/${Repository}/refs/heads/main/${TestDataPath}/${UrlTestFile}"

	$ProgressPreference = 'SilentlyContinue'
	Invoke-WebRequest -Uri ${Url} -OutFile "${TestInputDirectory}\${TestSet}\${TestFile}"
}

