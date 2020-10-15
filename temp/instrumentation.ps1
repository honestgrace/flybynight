$files = Get-ChildItem
$files | foreach {
$file = $PSItem

# $file = $files[0]
$content = (Get-Content $file).Split("`r`n")
$content | foreach {
    $line = $PSItem.Trim()
    if ($line.EndsWith(")") -and $line.Contains("("))
    {
        #if ($line.Contains(">") -or $line.Contains("+") -or $line.Contains("!") -or $line.Contains("&") -or $line.Contains(":") -or $line.Contains("#") -or $line.Contains("=") -or $line.Contains("if") -or $line.Contains("for") -or $line.Contains("//"))
        if ($line.StartsWith("//"))
        {
            #$line
        }
        elseif ($line.Contains("="))
        {
            #$line
        }
        elseif ($line.Contains("#"))
        {
            #$line
        }
        elseif ($line.Contains("+"))
        {
            #$line
        }
        elseif ($line.StartsWith(":"))
        {
            #$line
        }
        elseif ($line.StartsWith("!"))
        {
            #$line
        }
        elseif ($line.StartsWith("`)"))
        {
            #$line
        }
        elseif ($line.StartsWith("?"))
        {
            #$line
        }
        elseif ($line.StartsWith("sizeof`(") -or $line.StartsWith("sizeof "))
        {
            #$line
        }
        elseif ($line.StartsWith("if`(") -or $line.StartsWith("if "))
        {
            #$line
        }
        elseif ($line.StartsWith("switch`(") -or $line.StartsWith("switch "))
        {
            #$line
        }
        elseif ($line.StartsWith("else`(") -or $line.StartsWith("else "))
        {
            #$line
        }
        elseif ($line.StartsWith("while`(") -or $line.StartsWith("while "))
        {
            #$line
        }
        elseif ($line.StartsWith("for`(") -or $line.StartsWith("for "))
        {
            #$line
        }
        else
        {
            $line
        }
    }
}

}