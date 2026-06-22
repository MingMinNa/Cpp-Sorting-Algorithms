# tests.ps1

$CXX = "g++"
$CXXFLAGS = @(
    "-std=c++23"
    "-Wall"
    "-Wextra"
    "-Wpedantic"
)

# Find test source files
$testSrcs = Get-ChildItem "tests" -Filter "*_test.cpp"

if (-not $testSrcs) {
    Write-Host "No test files found."
    exit 1
}

New-Item -ItemType Directory -Force build | Out-Null

$testBins = @()

# Compile
foreach ($src in $testSrcs) {

    $name = [System.IO.Path]::GetFileNameWithoutExtension($src.Name)

    if ($IsWindows) {
        $bin = "build/$name.exe"
    }
    else {
        $bin = "build/$name"
    }

    Write-Host "Compiling $($src.Name)..."

    & $CXX $src.FullName @CXXFLAGS -o $bin

    if ($LASTEXITCODE -ne 0) {
        Write-Host "Compilation failed."
        exit 1
    }

    $testBins += $bin
}

# Run tests
$total  = 0
$passed = 0
$failed = @()

foreach ($bin in $testBins) {

    $total++

    Write-Host ""
    Write-Host "========== $bin =========="

    & $bin

    if ($LASTEXITCODE -eq 0) {
        $passed++
    }
    else {
        $failed += ([System.IO.Path]::GetFileName($bin))
    }
}

# Summary
Write-Host "Summary: $passed/$total tests passed."

if ($failed.Count -eq 0) {
    Write-Host "All tests passed."
}
else {

    Write-Host "Failed test files:"

    foreach ($f in $failed) {
        Write-Host "  - $f"
    }

    exit 1
}