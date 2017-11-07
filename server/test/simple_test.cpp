//
// Created by 49844 on 29/10/2017.
//

#include <thread>
#include "simple_client.hh"
#include "gtest/gtest.h"

TEST(example, add)
{
    asio::io_service ioService;
    tcp::resolver resolver{ioService};
    auto endpointIt = resolver.resolve({ "localhost", "4242" });
    test::SimpleClient client{ioService, endpointIt};

    std::thread thread{ [&ioService] () { ioService.run(); } };

    client.stop();
    thread.join();

    EXPECT_EQ(1, 1);
}