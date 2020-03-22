# A Matching Engine
A c++ matching engine with order book, accept stdin to add orders, output trade/cancel/fill/partialfill event

# Features
* For two major operations in order, O(1) to cancel, O(1) to match, O(1) to add if already have price level or O(logM) otherwise. Assume M is the average number of quotes in the order book
* No double or float comparison
* Memory efficient 

# Install
`$ make`

# Run Unit Test
`$ make test`

# Run Performance Test
`$ make perf`

# Run the matching engine
`$ ./run.sh`
