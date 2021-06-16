@REM ASCII=7bit ascii(no bom), UTF8=with bom marker
set cmd=^
  $file = '%1'; ^
  $old = '%2'; ^
  $new = '%3'; ^
  [string[]]$data = Get-Content $file; ^
  $data = $data -replace $old, $new; ^
  out-file -InputObject $data -encoding UTF8 -filepath $file;
powershell -NoLogo -Noninteractive -InputFormat none -Command "%cmd%"