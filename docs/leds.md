# Descripción del comportamiento de los LEDs

## Led 1
Alimentacion externa, bateria, error grave (por ejemplo SIM error)

## Led 2
Indica el estado de la red GSM
- **stage 1**: cuando el modulo contesta AT
- **stage 2**: cuando llega 'sim ready'
- **stage 3**: cuando se registro en la red
- **stage 4**: cuando le asignaron ip y va a abrir el socket
- **lit**: cuando llega el ACK de la trama actual.

## Led 3
Indica el estado del módulo de comunicaciones.
Posee 4 estados representados por diferentes secuencias, de acuerdo a los estados de `CommMgr`:
- **nolit**: en los estados `Idle` y `Disconnected`. Red no disponible
- **stage 1**: en el estado `WaitSync`. Espera para enviar estado actual e historia (por defecto 60s)
- **lit**: en el estado `Current`. Enviando estado actual
- **stage 4**: en el estado `History`. Enviando historia

## Led 4
Indica el estado del GPS
- **stage 2**: sin señal gps
- **lit**: con señal gps

## Led 5
Posee 2 estados (por el momento), de acuerdo a los 2 estados disponibles de la región `Mapping` de `Collector`:
- **nolit**: en el estado `Stopped`. No mapea
- **lit**: en el estado `Running`. Mapeando

## Led 6
Indica el estado de la SIM card
*(Necesitaríamos mostrar con que sim esta conectado, ejemplo:)*
- **stage 1**: Movistar
- **stage 2**: Claro
