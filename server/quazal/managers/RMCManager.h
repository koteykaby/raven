// RMC (Remote method call) implementation
#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

namespace Quazal
{
    class RMC {
    public:
        uint32_t size;
        uint8_t proto;
        uint8_t protoId;
        bool isRequest;
        uint16_t extendedProtoId;
        uint8_t isSuccess;
        uint32_t errorCode;
        uint32_t callID;
        uint32_t methodID;
        std::vector<uint8_t> payload;

        RMC() = default;
        static RMC from_bytes(const std::vector<uint8_t>& data);
        std::vector<uint8_t> to_bytes() const;
    };

    // Base RMC class to create handlers faster
    class RMCBaseHandler {
    public:
        virtual ~RMCBaseHandler() = default;
        // Method to get proto name
        virtual std::string GetProtocolName() const = 0;
        // Method to get method name
        virtual std::string GetMethodName(uint32_t method_id) const = 0;
        // Method to create payload
        virtual std::vector<uint8_t> CreatePayload(RMC& request) = 0;
    };

    // Class to dispatch RMC's
    class RMCDispatcher {
        std::string service_name;
    public:
        RMCDispatcher() = default;
        RMCDispatcher(const std::string& serviceName);;
        // Register new handler for a certain proto
        void RegisterHandler(uint16_t protocol_id, std::shared_ptr<RMCBaseHandler> handler);
        std::vector<uint8_t> ProcessRequest(RMC& request);
    private:
        std::unordered_map<uint16_t, std::shared_ptr<RMCBaseHandler>> handlers;

        // Find handler by ID
        std::shared_ptr<RMCBaseHandler> find_handler(uint16_t protocol_id);
    };
}