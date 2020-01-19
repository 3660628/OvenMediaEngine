//
// Created by getroot on 19. 12. 9.
//

#pragma once

#include <base/common_types.h>
#include <base/ovlibrary/url.h>

#include <base/provider/stream.h>
#include <base/ovlibrary/semaphore.h>
#include <base/provider/application.h>
#include <modules/ovt_packetizer/ovt_packet.h>
#include <modules/ovt_packetizer/ovt_depacketizer.h>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/pixdesc.h>
#include <libavutil/opt.h>
}

namespace pvd
{
	class RtspcStream : public pvd::Stream
	{
	public:
		static std::shared_ptr<RtspcStream>
		Create(const std::shared_ptr<pvd::Application> &application, const ov::String &stream_name,	const std::vector<ov::String> &url_list);

		RtspcStream(const std::shared_ptr<pvd::Application> &application, const StreamInfo &stream_info, const std::vector<ov::String> &url_list);

		~RtspcStream() final;

	private:
		bool Start() override;
		bool Stop() override;
		void WorkerThread();
		bool ConnectOrigin();
		bool RequestDescribe();
		bool ReceiveDescribe(uint32_t request_id);
		bool RequestPlay();
		bool ReceivePlay(uint32_t request_id);
		bool RequestStop();
		bool ReceiveStop(uint32_t request_id);

		std::shared_ptr<OvtPacket> ReceivePacket();
		std::shared_ptr<ov::Data> ReceiveMessage();

		std::vector<std::shared_ptr<const ov::Url>> _url_list;
		std::shared_ptr<const ov::Url>				_curr_url;
		bool _stop_thread_flag;
		std::thread _worker_thread;

		ov::Socket _client_socket;

		uint32_t _last_request_id;
		uint32_t _session_id;

		OvtDepacketizer _depacketizer;
	};
}