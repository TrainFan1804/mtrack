# Response codes overview

Legend:
- F     := Frontend
- B     := Backend
- F/B   := Frontend and Backend


| Code 	| Meaning 	                            | Sender    |
|:----:	|:---------                             |:--------:	|
|100    |Ask for data                           |F          |
|101    |Adding a new media                     |F          |
|102    |Removing a media                       |F          |
|200    |Sending data from database             |B          |
|300    |Transaction end                        |B          |
|400    |Wrong response format                  |F/B        |
|500    |Database error                         |B          |


## Examples

- Startup:                  <100/F> -> <200/B> -> <300/F>
- Adding a new media:       <101/F> -> <300/F>
- Removing a media:         <102/F> -> <300/F>
- F send wrong format:      <000/F> -> <400/B>
- Removing unknown media:   <102/F> -> <500/B>
