# D&D Initiative Tracker

Ein einfacher Initiative-Tracker für Dungeons & Dragons Spielleiter, entwickelt mit C++ und Qt.

## Funktionen

- Hinzufügen von Charakteren mit Namen und Initiative-Modifikatoren
- Automatisches Würfeln der Initiative für alle Charaktere
- Sortierte Anzeige der Charaktere nach Initiative-Wert
- Entfernen einzelner Charaktere oder Leeren der gesamten Liste

## Kompilierung

### Voraussetzungen

- CMake (Version 3.16 oder höher)
- Qt 5.15 oder Qt 6
- C++ Compiler mit C++17-Unterstützung

### Unter macOS

1. Installieren Sie die Abhängigkeiten mit Homebrew:
   ```
   brew install cmake qt@6
   ```

2. Klonen Sie das Repository:
   ```
   git clone https://github.com/yourusername/DnDInitiativeTracker.git
   cd DnDInitiativeTracker
   ```

3. Erstellen Sie ein Build-Verzeichnis und kompilieren Sie:
   ```
   mkdir build
   cd build
   cmake ..
   make
   ```

4. Führen Sie die Anwendung aus:
   ```
   ./DnDInitiativeTracker
   ```

### Unter Windows

1. Installieren Sie Qt und CMake
2. Öffnen Sie die CMake-GUI, wählen Sie das Quellverzeichnis und ein Build-Verzeichnis
3. Konfigurieren und generieren Sie das Projekt
4. Öffnen Sie das generierte Projekt in Ihrer IDE und kompilieren Sie es

## Verwendung

1. Starten Sie die Anwendung
2. Geben Sie den Namen und den Initiative-Modifikator eines Charakters ein und klicken Sie auf "Hinzufügen"
3. Wiederholen Sie Schritt 2 für alle Charaktere und NPCs
4. Klicken Sie auf "Initiative würfeln", um für alle Charaktere zu würfeln
5. Die Tabelle wird automatisch nach den Ergebnissen sortiert, wobei der höchste Wert oben steht

## Lizenz

Dieses Projekt steht unter der MIT-Lizenz - siehe die [LICENSE](LICENSE) Datei für Details. 