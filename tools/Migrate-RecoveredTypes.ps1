param(
    [Parameter(Mandatory)][string]$Root,
    [Parameter(Mandatory)][string]$HeaderRoot,
    [switch]$Apply,
    [string[]]$Include = @('*'),
    [string[]]$Exclude = @(),
    [string]$ReportPath
)

$ErrorActionPreference = 'Stop'
$Root = [IO.Path]::GetFullPath($Root)
$HeaderRoot = [IO.Path]::GetFullPath($HeaderRoot)
if(-not (Test-Path -LiteralPath $Root -PathType Container)) { throw "Source root not found: $Root" }
if(-not (Test-Path -LiteralPath $HeaderRoot -PathType Container)) { throw "Header root not found: $HeaderRoot" }

$headerFiles = @(Get-ChildItem -LiteralPath $HeaderRoot -Recurse -File | Where-Object Extension -In @('.h', '.hpp'))
$canonical = [Collections.Generic.HashSet[string]]::new([StringComparer]::Ordinal)
$canonicalPattern = '\b[A-Za-z_][A-Za-z_0-9]*?(?:u[0-3]|s[0-3]|f[23])_[tc]\b'
foreach($header in $headerFiles) {
    $source = [IO.File]::ReadAllText($header.FullName)
    foreach($match in [regex]::Matches($source, $canonicalPattern)) { [void]$canonical.Add($match.Value) }
}

$widths = @{ '8' = '0'; '16' = '1'; '32' = '2'; '64' = '3' }
$extra = @{
    'vcuc' = 'vcu0_t'
    'vcs' = 'vcst_t'
    'vcvcs' = 'vcvcst_t'
    'vvcs' = 'vvcst_t'
    'vs' = 'vstr_t'
    'b82vcs' = 'b82st'
    'vcs2b8' = 'st2b8'
    'VCC_NULL' = 'VCST_NULL'
    'VCC_TRUE' = 'VCST_TRUE'
    'VCC_FALSE' = 'VCST_FALSE'
    'astvcs' = 'astvcst_t'
    'astaticvcs' = 'astvcst_t'
    'astvs' = 'astvstr_t'
    'astaticvs' = 'astvstr_t'
    'astuc' = 'astuc_t'
    'astaticuc' = 'astuc_t'
    'astuchar' = 'astuc_t'
    'astc' = 'astsc_t'
    'astaticc' = 'astsc_t'
    'astchar' = 'astsc_t'
    'astvcuc' = 'astvcu0_t'
    'astaticvcuc' = 'astvcu0_t'
    'astvcc' = 'astvcst_t'
    'astaticvcc' = 'astvcst_t'
    'astaticvcu0_t' = 'astvcu0_t'
}

function Resolve-CanonicalName([string]$name) {
    if($extra.ContainsKey($name)) { return $extra[$name] }
    $parts = [regex]::Match($name, '^(?<object>[A-Za-z_][A-Za-z_0-9]*?)(?<kind>u|i|f)(?<bits>8|16|32|64)(?<const>_c)?$')
    if(-not $parts.Success) { return $null }
    $kind = switch($parts.Groups['kind'].Value) { 'i' { 's' } default { $parts.Groups['kind'].Value } }
    $bits = $parts.Groups['bits'].Value
    $index = if($kind -eq 'f') { $bits.Substring(0, 1) } else { $widths[$bits] }
    if($kind -eq 'f' -and $bits -notin @('32', '64')) { return $null }
    if($kind -eq 'f') { $index = if($bits -eq '32') { '2' } else { '3' } }
    $qualifier = if($parts.Groups['const'].Success) { 'c' } else { 't' }
    $object = $parts.Groups['object'].Value
    if($object.StartsWith('astatic', [StringComparison]::Ordinal)) { $object = 'ast' + $object.Substring(7) }
    $candidate = $object + $kind + $index + '_' + $qualifier
    if(-not $canonical.Contains($candidate)) { return $null }
    $shape = @('circle', 'sphere', 'rect2', 'rect3', 'rect', 'tri2', 'tri3', 'tri', 'n2', 'n3') | Where-Object { $_ -ceq $object -or ('v' + $_) -ceq $object -or ('vc' + $_) -ceq $object -or ('a' + $_) -ceq $object } | Select-Object -First 1
    if($shape) {
        $type = $shape + '<' + $kind + $index + '_t>'
        if($object -eq ('v' + $shape)) { $type = 'view<' + $type + '>' }
        elseif($object -eq ('vc' + $shape)) { $type = 'view<cnst ' + $type + '>' }
        elseif($object -eq ('a' + $shape)) { $type = 'apod<' + $type + '>' }
        if($parts.Groups['const'].Success) { $type = 'cnst ' + $type }
        return $type
    }
    if($object -eq 'v' -or $object -eq 'vc') {
        $element = $kind + $index + '_t'
        if($object -eq 'vc') { $element = 'cnst ' + $element }
        $type = 'view<' + $element + '>'
        if($parts.Groups['const'].Success) { $type = 'cnst ' + $type }
        return $type
    }
    if($object -eq 'a') {
        $type = 'apod<' + $kind + $index + '_t>'
        if($parts.Groups['const'].Success) { $type = 'cnst ' + $type }
        return $type
    }
    if($canonical.Contains($candidate)) { return $candidate }
    return $null
}

# The alternatives before `id` protect comments and quoted text from replacement.
$tokens = [regex]::new('(?s)/\*.*?\*/|//[^\r\n]*|R"(?<delimiter>[^ ()\\\t\r\n]{0,16})\(.*?\)\k<delimiter>"|"(?:\\.|[^"\\])*"|''(?:\\.|[^''\\])*''|(?<id>\b[A-Za-z_][A-Za-z_0-9]*\b)')
$encoding = [Text.UTF8Encoding]::new($false)
$report = [Collections.Generic.List[object]]::new()
$files = @(Get-ChildItem -LiteralPath $Root -Recurse -File | Where-Object {
    if($_.Extension -notin @('.h', '.hpp', '.c', '.cpp', '.cxx')) { return $false }
    $relative = $_.FullName.Substring($Root.TrimEnd('\').Length).TrimStart('\')
    $selected = @($Include | Where-Object { $relative -like $_ }).Count -gt 0
    $omitted = @($Exclude | Where-Object { $relative -like $_ }).Count -gt 0
    return $selected -and -not $omitted
})
foreach($file in $files) {
    $before = [IO.File]::ReadAllText($file.FullName)
    $changes = [Collections.Generic.List[object]]::new()
    $after = $tokens.Replace($before, [Text.RegularExpressions.MatchEvaluator]{
        param($match)
        if(-not $match.Groups['id'].Success) { return $match.Value }
        $lineStart = $before.LastIndexOf("`n", [Math]::Max(0, $match.Index - 1)) + 1
        $lineEnd = $before.IndexOf("`n", $match.Index)
        if($lineEnd -lt 0) { $lineEnd = $before.Length }
        $lineText = $before.Substring($lineStart, $lineEnd - $lineStart)
        if($lineText -match '^\s*(?:tydf|using|tplN2uusng|#\s*define)\b') { return $match.Value }
        $replacement = Resolve-CanonicalName $match.Value
        if(-not $replacement -or $replacement -ceq $match.Value) { return $match.Value }
        $line = 1 + ([regex]::Matches($before.Substring(0, $match.Index), '\n')).Count
        $changes.Add([pscustomobject]@{ line = $line; from = $match.Value; to = $replacement })
        return $replacement
    })
    if($changes.Count -eq 0) { continue }
    if($Apply) { [IO.File]::WriteAllText($file.FullName, $after, $encoding) }
    $relative = $file.FullName.Substring($Root.TrimEnd('\').Length).TrimStart('\')
    $report.Add([pscustomobject]@{ file = $relative; changes = @($changes) })
    Write-Output "$relative : $($changes.Count) replacements"
    foreach($change in $changes) { Write-Output "  $($change.line): $($change.from) -> $($change.to)" }
}
if($ReportPath) {
    $ReportPath = [IO.Path]::GetFullPath($ReportPath)
    $parent = Split-Path -Parent $ReportPath
    if(-not (Test-Path -LiteralPath $parent -PathType Container)) { throw "Report directory not found: $parent" }
    [IO.File]::WriteAllText($ReportPath, (($report | ConvertTo-Json -Depth 5) + "`n"), $encoding)
}
Write-Output "Mode: $(if($Apply) {'apply'} else {'review'}); files scanned: $($files.Count); files changed: $($report.Count); known canonical types: $($canonical.Count)."
