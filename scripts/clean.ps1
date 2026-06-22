# clean.ps1

if (Test-Path build) {
    Remove-Item build -Recurse -Force
}

Get-ChildItem -Filter "*.out" | Remove-Item -Force -ErrorAction SilentlyContinue
Get-ChildItem -Filter "*.o"   | Remove-Item -Force -ErrorAction SilentlyContinue
Get-ChildItem -Filter "*.exe" | Remove-Item -Force -ErrorAction SilentlyContinue

Write-Host "Clean complete."