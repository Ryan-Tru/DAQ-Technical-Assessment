# Brainstorming

This file is used to document your thoughts, approaches and research conducted across all tasks in the Technical Assessment.

## Firmware

#### STAGE 1
I had some trouble setting/building libraries.
like not finding Boost package for cmake.

I figured that installing it into the docker image using apt-get PACKAGE would
fix the issue, as it was missing dependencies. I googled the error message and
found some StackOverflow posts explaining some fixes. I first thought that it
was not finding some packages as it was named wrong (Boost_INCLUDE_DIR instead
of Boost_INCLUDEDIR), but that was not the issue. so I just ended up installing
it in the docker image and it worked :).

I also had some troubles getting CMAKE started, as I was confused whether to
just work on the one in `solutions/CMakeLists` or the `firmware/CMakeLists`, but I
decided to use both, as `dbcppp` lives in the firmware dir. I just linked it
inside the `solutions/CMakeLists`. 

[I used this cmake tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/Adding%20a%20Library.html)

As for storing can frames I am using unordered maps like the dbcppp example. 

Currently I am having trouble with storing them, as doing 
`std::unique_ptr<dbcppp::INetwork> net;` creates a unique ptr (I am trying to
populate multiple maps for each DBC file from a function.) meaning that it gets
destroyed after I return from the function.

Deepseek recommends that I return net and the message map as a pair using
`std::pair`, so I can transfer the ownership of net to the caller using
std::move. 

```
    std::string payload{};
    if (idAndPayloadVec.size() > 1) {
         payload = idAndPayloadVec[1];
```

Before this piece of code, I was just trying to get the payload without
checking that it existed, causing seg faults.

```
    auto findMsg = messageMap.find(id);

    if (findMsg != messageMap.end()) {
        const dbcppp::IMessage* msg = findMsg->second;
```

same for this piece of code directly below that. I was trying to access some
ids that did not exist in the map. leading me to find out about the find method
in unordered_maps. found here [unordered map docs](https://en.cppreference.com/w/cpp/container/unordered_map.html)

Other than that I just followed along with what the dbcppp example did. (had to
do some digging aroung in the .h files to find methods and whatnot.)

I had some trouble formatting the rawtophys value as well, but I searched
through SO and found these:

[Double formatting 1](https://stackoverflow.com/questions/19135953/formatting-a-double-value-to-1-decimal-place) 

and 

[Double formatting 2](https://stackoverflow.com/questions/73752186/how-do-you-use-setprecision-when-declaring-a-double-variable-in-c)

I also decided to use docker cp to get the output file, as seemed more simple.

When I checked the output.txt file I found that I was getting 0.0 and -0.0. I
printed out the raw values and found that it was giving me 0.000001 etc. At
first, I tried to see if checking val > -0.9999999999 && val < 0.9999999999
would work, but I realise that you could have values that do not satisfy those
conditions, so I decided instead to cast it to an int val, and check it against
zero. (int casting from double just drops whatever is after the point e.g.
0.0001 becomes 0)

#### STAGE 2 
LINKS I USED TO RESEARCH:
[SPI Vs CAN](https://embetronicx.com/uncategorized/spi-vs-can-choosing-the-right-protocol-for-your-project/#SPI_vs_CAN)

[5 Advantages Of CAN](https://www.totalphase.com/blog/2019/08/5-advantages-of-can-bus-protocol/)

[PCIe wiki](https://en.wikipedia.org/wiki/PCI_Express)

[CAN Bus wiki](https://en.wikipedia.org/wiki/CAN_bus)

the ST website is designed so bad :(    

#### STAGE 3 
I don't think I will have time to finish all the tests :( 

To run tests:
docker compose up
docker exec -it {container-name} bash 
cd build/tests 
./tests


## Spyder

## Cloud
