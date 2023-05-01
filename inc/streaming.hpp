#pragma once
#include "serializer.hpp"

class Stream
{

public:

    void loadStreaming();
    void loadNetwork();



private:
    std::unique_ptr<i_Serializer> m_serializer = std::make_unique<SerializerToJSON>();

};
