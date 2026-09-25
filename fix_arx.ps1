$files = @("game.cpp","tavern.cpp","inn.cpp","smithy.cpp","bank.cpp","retreat.cpp","guild.cpp","encounter.cpp","createCharacter.cpp","lyrics.cpp")
foreach ($f in $files) {
    if (Test-Path $f) {
        $c = Get-Content $f -Raw -Encoding UTF8
        $c = $c -replace 'arx::Clock', 'sf::Clock'
        $c = $c -replace 'arx::Time::Zero', 'sf::Time::Zero'
        $c = $c -replace 'arx::Time', 'sf::Time'
        $c = $c -replace 'arx::seconds', 'sf::seconds'
        $c = $c -replace 'arx::milliseconds', 'sf::milliseconds'
        $c = $c -replace 'arx::sleep', 'sf::sleep'
        Set-Content $f -Value $c -Encoding UTF8 -NoNewline
        Write-Host "Fixed: $f"
    }
}
