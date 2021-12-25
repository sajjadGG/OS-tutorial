## Setup

first create file named `myfifo` in `/tmp` to be used as named pipe
(this can be changed in `logheader.h`)
run `make` to create `./logger` , `./producer`
then first run `./logger` to see the message `logger: hello world`
then you can run multiple `./producer` in several terminal and start writing your message
