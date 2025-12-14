#include "RMCManager.h"

#include "stream.h"
#include "spdlog/spdlog.h"

#include <memory>
#include <unordered_map>
#include <cstring>

namespace Quazal
{
    // summaries by chatgpt

    // Parse RMC from bytes
    /**
     * @brief Deserializes RMC (Remote Method Call) data from a byte vector
     * 
     * @details Parses binary data into an RMC structure according to the following format:
     * - 4 bytes: size
     * - 1 byte: protocol ID
     *   - If 0x7F: next 2 bytes are extended protocol ID
     *   - Otherwise: lowest 7 bits are protocol ID, highest bit indicates if request
     * - 4 bytes: method ID
     * - Remaining bytes: payload data
     *
     * @param data Vector containing the binary RMC data to parse
     * @return RMC Parsed RMC structure containing protocol info, method ID and payload
     */
    RMC RMC::from_bytes(const std::vector<uint8_t>& data)
    {
        RMC rmc;
        Stream rdr(data);

        rmc.size = rdr.read<uint32_t>();

        uint8_t proto = rdr.read<uint8_t>();

        if (proto == 0x7F) {
            rmc.extendedProtoId = rdr.read<uint16_t>();
        } else {
            rmc.protoId = proto & 0x7F;
            rmc.isRequest = (proto & 0x80) != 0;
        }
        
        rmc.callID = rdr.read<uint32_t>();
        rmc.methodID = rdr.read<uint32_t>();

        size_t remaining_bytes = rdr.remaining();
        rmc.payload = rdr.read_bytes(remaining_bytes);

        SPDLOG_DEBUG("RMC from_bytes: proto={} protoID={} isRequest={} callID={} methodID={}",
            proto, rmc.protoId, rmc.isRequest, rmc.callID, rmc.methodID);

        return rmc;
    }
    // Serialize RMC to bytes
    /**
     * @brief Serializes the RMC message into a byte vector
     * 
     * Serialization format:
     * - uint32_t: Total message size
     * - uint8_t: Protocol ID (If > 0x7E, writes 0x7F followed by extended protocol ID)
     *           For regular protocol IDs, MSB indicates if message is request
     * 
     * For requests:
     * - uint32_t: Call ID
     * - uint32_t: Method ID
     * - vector: Payload data
     * 
     * For responses:
     * - uint8_t: Success flag (1=success, 0=error)
     * If success:
     *   - uint32_t: Call ID
     *   - uint32_t: Method ID (with 0x8000 flag set)
     *   - vector: Response payload
     * If error:
     *   - uint32_t: Error code
     *   - uint32_t: Call ID
     * 
     * @return std::vector<uint8_t> Serialized RMC message as byte vector
     */
    std::vector<uint8_t> RMC::to_bytes() const
    {
        Stream wtr;

        // The first field is uint32_t size
        // But it will be calculated and placed later
        uint8_t proto = protoId;

        if (protoId > 0x7E) {
            wtr.write(uint8_t(0x7F));
            wtr.write(extendedProtoId);
        } else {
            if (isRequest) {
                proto |= 0x80;
            }
            wtr.write(proto);
        }

        if (isRequest) {
            wtr.write<uint32_t>(callID);
            wtr.write<uint32_t>(methodID);
            wtr.write_vector(payload);
        } else {
            wtr.write<uint8_t>(isSuccess ? 1 : 0);
            if (isSuccess) {
                wtr.write<uint32_t>(callID);
                wtr.write<uint32_t>(methodID | 0x8000);
                wtr.write_vector(payload);
            } else {
                wtr.write<uint32_t>(errorCode);
                wtr.write<uint32_t>(callID);
            }
        }

        SPDLOG_DEBUG("RMC to_bytes: proto={} protoID={} isRequest={} callID={} methodID={}",
            proto, protoId, isRequest, callID, methodID);

        std::vector<uint8_t> serialized_data = wtr.get_data();

        uint32_t size = static_cast<uint32_t>(serialized_data.size());

        Stream result;

        // Writing size
        result.write(size);
        result.write_vector(serialized_data);

        return result.get_data();
    }

    // I don't think that switch case or if if if is very good idea for RMC protos dispatching....
    // I'd prefer to use objected-oriented programming strength to develop cool thing that will dispatch protos
    // even without my knowledge about it. 
    // of course except logging. i'll see all........
    // i'll spy on you.

    RMCDispatcher::RMCDispatcher(const std::string& serviceName)
        : service_name(serviceName)
    {
        service_name = serviceName;
        SPDLOG_INFO("{}: Created new RMC dispatcher", service_name);
    }

    std::shared_ptr<RMCBaseHandler> RMCDispatcher::find_handler(uint16_t protocol_id)
    {
        if (auto it = handlers.find(protocol_id); it != handlers.end()) {
            SPDLOG_DEBUG("{}: Looking for a proto {}", service_name, protocol_id);
            return it->second;
        }
        SPDLOG_WARN("{}: There's no proto such as {}", service_name, protocol_id);
        return nullptr;
    }

    // Registering protocol handler
    void RMCDispatcher::RegisterHandler(uint16_t protocol_id, std::shared_ptr<RMCBaseHandler> handler)
    {
        handlers[protocol_id] = std::move(handler);
        SPDLOG_INFO("{}: Registered handler for proto {}", service_name, protocol_id);
    }

    std::vector<uint8_t> RMCDispatcher::ProcessRequest(RMC& request)
    {
        SPDLOG_INFO("{}: Looking for a protocol {}, method {}", service_name, request.protoId, request.methodID);

        auto handler = find_handler(request.protoId);
        if (handler) {
            SPDLOG_INFO("{}: Calling {}.{}", service_name, handler->GetProtocolName(), handler->GetMethodName(request.methodID));
            return handler->CreatePayload(request);
        }

        SPDLOG_ERROR("{}: No handler available for protocol {} method {}", service_name, request.protoId, request.methodID);
        return {};
    }
}