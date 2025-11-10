# Stop on any error
$ErrorActionPreference = "Stop"

# ======================================
# Configuration
# ======================================
$BuildDir = "build"
$GeneratorDir = Join-Path $BuildDir "generator"
$TesterDir    = Join-Path $BuildDir "tester"
$GeneratorExe = "generator.exe"   # relative to $GeneratorDir
$TesterExe    = "tester.exe"      # relative to $TesterDir

# ======================================
# Build
# ======================================
Write-Host "Building project..."
cmake -S . -B $BuildDir -DCMAKE_BUILD_TYPE=Debug
cmake --build $BuildDir --parallel

# ======================================
# Run generator
# ======================================
$generatorPath = Join-Path $GeneratorDir $GeneratorExe
if (Test-Path $generatorPath) {
    Write-Host "Running generator..."
    Push-Location $GeneratorDir
    & .\$GeneratorExe
    Pop-Location
} else {
    Write-Error "Generator binary not found at: $generatorPath"
    exit 1
}

# ======================================
# Run tester
# ======================================
$testerPath = Join-Path $TesterDir $TesterExe
if (Test-Path $testerPath) {
    Write-Host "Running tester..."
    Push-Location $TesterDir
    & .\$TesterExe
    Pop-Location
} else {
    Write-Error "Tester binary not found at: $testerPath"
    exit 1
}

Write-Host "All done!"
