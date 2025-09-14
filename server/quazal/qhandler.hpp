#pragma once

#include "qpacket.hpp"
#include "ioutils.hpp"
#include "typeflags.hpp"
#include "vport.hpp"
#include "clientinfo.hpp"

#include "../macros.hpp"

namespace quazal
{
	std::vector<uint8_t> handle_SYN(qpacket req, sockaddr_in addr)
	{
		qpacket resp;

		resp.source = req.destination;
		resp.destination = req.source;

		resp.packetTypeFlags = utils::CreateTypeFlagsByte(
			PacketType::SYN,
			PacketFlags::ACK | PacketFlags::HAS_SIZE
		);

		resp.sessionId = req.sessionId;
		resp.signature = req.signature;
		resp.sequenceId = req.sequenceId;
		resp.connSignature = RegisterClient(addr);
		resp.payloadSize = 0x0000;

		std::vector<uint8_t> result = resp.serialize();

		return result;
	}

	std::vector<uint8_t> handle_CONNECT(qpacket req)
	{
		qpacket resp;

		resp.source = req.destination;
		resp.destination = req.source;

		resp.packetTypeFlags = utils::CreateTypeFlagsByte(
			PacketType::CONNECT,
			PacketFlags::ACK
		);
		
		resp.sessionId = req.sessionId;
		resp.signature = req.connSignature.value(); // for some reason it just swaps, idk what are they actually mean
		resp.sequenceId = req.sequenceId;
		resp.connSignature = req.signature; // uhhuhuhuhu 

		std::vector<uint8_t> result = resp.serialize();
		return result;
	}

	std::vector<uint8_t> handle_PING(qpacket req)
	{
		qpacket resp;

		resp.source = req.destination;
		resp.destination = req.source;

		resp.packetTypeFlags = utils::CreateTypeFlagsByte(
			PacketType::PING,
			PacketFlags::ACK
		);

		resp.sessionId = req.sessionId;
		resp.signature = req.signature;
		resp.sequenceId = req.sequenceId;

		std::vector<uint8_t> result = resp.serialize();

		return result;
	}

	std::vector<uint8_t> handle_DATA(qpacket req)
	{
		qpacket resp;

		resp.source = req.destination;
		resp.destination = req.source;

		resp.packetTypeFlags = utils::CreateTypeFlagsByte(
			PacketType::DATA,
			PacketFlags::ACK | PacketFlags::HAS_SIZE
		);

		resp.sessionId = req.sessionId;
		resp.signature = req.signature;
		resp.sequenceId = req.sequenceId;
		resp.fragmentId = 0;
		resp.payloadSize = 0;

		std::vector<uint8_t> result = resp.serialize();

		return result;
	}

	std::vector<uint8_t> handle(qpacket req, sockaddr_in addr)
	{
		PacketType type = utils::ReadPacketType(req.packetTypeFlags);

		std::vector<uint8_t> result;

		switch (type) {
			case PacketType::SYN: 
				result = handle_SYN(req, addr);
				break;
			case PacketType::CONNECT:
				result = handle_CONNECT(req);
				break;
			case PacketType::PING:
				result = handle_PING(req);
				break;
			case PacketType::DATA:
				result = handle_DATA(req);
				break;
			// If packet type unknown just ignore it. Game shouldn't send any more specific.
			default:
				LOG_WARNING("Unable to handle packet type {}", static_cast<int>(type));
				break;
		}

		return result;
	}
}