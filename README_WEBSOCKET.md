# WebSocket-Kommunikation für D&D Initiative Tracker

Diese Dokumentation beschreibt, wie man die WebSocket-Schnittstelle des D&D Initiative Trackers nutzen kann, um mit der Anwendung zu kommunizieren.

## Übersicht

Der D&D Initiative Tracker bietet eine WebSocket-Schnittstelle, über die externe Anwendungen mit dem Tracker kommunizieren können. Die Kommunikation erfolgt über JSON-formatierte Nachrichten.

Der WebSocket-Server läuft auf `ws://localhost:8080`, wenn die Anwendung gestartet ist.

## Beispiel-Client

Im Repository ist eine HTML-Datei `websocket_client.html` enthalten, die als Beispiel-Client dient. Diese Datei kann in einem Webbrowser geöffnet werden, um mit dem D&D Initiative Tracker zu kommunizieren.

## Verfügbare Befehle

Die folgenden Befehle können über die WebSocket-Schnittstelle gesendet werden:

### Initiative würfeln

```json
{
  "command": "rollInitiative"
}
```

Dieser Befehl würfelt die Initiative für alle Charaktere im Tracker.

### Willenskraft würfeln

```json
{
  "command": "rollWillSave"
}
```

Dieser Befehl würfelt Willenskraft-Rettungswürfe für alle Charaktere im Tracker.

### Reflex würfeln

```json
{
  "command": "rollReflexSave"
}
```

Dieser Befehl würfelt Reflex-Rettungswürfe für alle Charaktere im Tracker.

### Konstitution würfeln

```json
{
  "command": "rollFortitudeSave"
}
```

Dieser Befehl würfelt Konstitution-Rettungswürfe für alle Charaktere im Tracker.

## Antworten

Der Server antwortet auf jeden Befehl mit einer JSON-Nachricht, die den Status und eine Meldung enthält:

```json
{
  "status": "success",
  "message": "Initiative für alle Charaktere gewürfelt"
}
```

Bei einem Fehler sieht die Antwort so aus:

```json
{
  "status": "error",
  "message": "Unbekannter Befehl: xyz"
}
```

## Integration in eigene Anwendungen

Um die WebSocket-Schnittstelle in eigene Anwendungen zu integrieren, kann der folgende JavaScript-Code als Ausgangspunkt dienen:

```javascript
// WebSocket-Verbindung herstellen
const socket = new WebSocket('ws://localhost:8080');

// Ereignishandler für die Verbindung
socket.onopen = () => {
  console.log('Verbindung hergestellt');
  
  // Beispiel: Initiative würfeln
  const message = JSON.stringify({ command: 'rollInitiative' });
  socket.send(message);
};

// Ereignishandler für empfangene Nachrichten
socket.onmessage = (event) => {
  console.log('Nachricht empfangen:', event.data);
  
  // Nachricht als JSON parsen
  const data = JSON.parse(event.data);
  
  // Auf die Antwort reagieren
  if (data.status === 'success') {
    console.log('Erfolg:', data.message);
  } else {
    console.error('Fehler:', data.message);
  }
};

// Ereignishandler für Fehler
socket.onerror = (error) => {
  console.error('WebSocket-Fehler:', error);
};

// Ereignishandler für das Schließen der Verbindung
socket.onclose = (event) => {
  console.log('Verbindung getrennt:', event.code);
};
```

## Erweiterungsmöglichkeiten

Die WebSocket-Schnittstelle kann leicht um weitere Befehle erweitert werden. Dazu muss die `processWebSocketMessage`-Methode in der `MainWindow`-Klasse angepasst werden.

## Fehlerbehebung

- Stelle sicher, dass der D&D Initiative Tracker läuft, bevor du versuchst, eine Verbindung herzustellen.
- Überprüfe, ob der Port 8080 auf deinem System verfügbar ist.
- Wenn du Probleme mit der Verbindung hast, überprüfe die Konsolenausgabe des Browsers (F12) auf Fehlermeldungen. 