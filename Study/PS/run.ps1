<#
    C++ 컴파일 + 실행 도우미 (Code Runner / 직접 실행 공용)

    사용법:
        .\run.ps1 01_string            # 같은 폴더의 01_string.cpp 컴파일 후 실행
        .\run.ps1 .\Cpp_Study\a.cpp    # 전체/상대 경로도 OK
        .\run.ps1 01_string -i in.txt  # in.txt 를 표준입력으로 넣어 실행

    같은 이름의 입력 파일(01_string.in / 01_string.txt / input.txt)이 있으면
    자동으로 표준입력으로 넣어줍니다. 없으면 키보드 입력.
#>
param(
    [Parameter(Mandatory = $true)][string]$File,
    [Alias('i')][string]$InFile
)

$ErrorActionPreference = 'Stop'
$gpp = 'C:\msys64\mingw64\bin\g++.exe'
# cc1plus 등 하위 프로세스가 DLL 을 찾도록 mingw64\bin 을 PATH 앞에 추가 (필수)
$env:PATH = 'C:\msys64\mingw64\bin;' + $env:PATH

# 경로 정리
$base = [System.IO.Path]::GetFileNameWithoutExtension($File)
$dir  = if ([System.IO.Path]::GetDirectoryName($File)) { [System.IO.Path]::GetDirectoryName($File) } else { $PSScriptRoot }
$src  = Join-Path $dir "$base.cpp"
$exe  = Join-Path $dir "$base.exe"

function Line { param($c = 'DarkGray') Write-Host ('─' * 50) -ForegroundColor $c }

if (-not (Test-Path $src)) { Write-Host "✗ 소스 파일 없음: $src" -ForegroundColor Red; exit 1 }

# 이전 실행이 입력 대기 등으로 안 끝나면 exe 가 잠겨 컴파일이 'Permission denied' 로 막힘 → 먼저 정리
Get-Process $base -ErrorAction SilentlyContinue | Stop-Process -Force -ErrorAction SilentlyContinue

# 컴파일 (C++17, 경고 켜고, 최적화)
Line
Write-Host "  ⚙  컴파일  " -ForegroundColor Cyan -NoNewline
Write-Host "$base.cpp" -ForegroundColor White
Line
$sw = [System.Diagnostics.Stopwatch]::StartNew()
& $gpp -std=c++17 -O2 -Wall -Wextra $src -o $exe
if ($LASTEXITCODE -ne 0) {
    Line Red
    Write-Host "  ✗ 컴파일 실패 (exit $LASTEXITCODE)" -ForegroundColor Red
    Line Red
    exit $LASTEXITCODE
}

# 입력 파일 자동 탐지
if (-not $InFile) {
    foreach ($cand in @("$base.in", "$base.txt", "input.txt")) {
        $p = Join-Path $dir $cand
        if (Test-Path $p) { $InFile = $p; break }
    }
}

$hasIn = $InFile -and (Test-Path $InFile)
Write-Host "  ▶  실행" -ForegroundColor Green -NoNewline
if ($hasIn) { Write-Host "   (입력: $(Split-Path $InFile -Leaf))" -ForegroundColor DarkGray }
else        { Write-Host "" }
Line

if ($hasIn) { Get-Content $InFile | & $exe } else { & $exe }
$code = $LASTEXITCODE
$sw.Stop()

Line
$col = if ($code -eq 0) { 'Green' } else { 'Red' }
$mark = if ($code -eq 0) { '✓' } else { '✗' }
Write-Host "  $mark 종료코드 $code" -ForegroundColor $col -NoNewline
Write-Host "   ·   $([int]$sw.Elapsed.TotalMilliseconds) ms" -ForegroundColor DarkGray
Line
