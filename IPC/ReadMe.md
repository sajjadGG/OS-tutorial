## Setup

Create below files for shared memory to use to store its values (System V)

- "/tmp/shared_memory_key"
- "/tmp/sem-mutex-key"
- "/tmp/sem-buffer-count-key"
- "/tmp/sem-spool-signal-key"

Next run `make` to create `./logger` , `./producer`
then first run `./logger` to see the message `logger: hello world`
then you can run multiple `./producer` in several terminal and start writing your message
