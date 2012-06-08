/**
 * Autogenerated by Thrift Compiler (0.8.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef SendJson_H
#define SendJson_H

#include <TProcessor.h>
#include "sendjson_types.h"

namespace sendjson {

class SendJsonIf {
 public:
  virtual ~SendJsonIf() {}
  virtual void SendJsonFileName(const std::vector<std::string> & filename) = 0;
};

class SendJsonIfFactory {
 public:
  typedef SendJsonIf Handler;

  virtual ~SendJsonIfFactory() {}

  virtual SendJsonIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(SendJsonIf* /* handler */) = 0;
};

class SendJsonIfSingletonFactory : virtual public SendJsonIfFactory {
 public:
  SendJsonIfSingletonFactory(const boost::shared_ptr<SendJsonIf>& iface) : iface_(iface) {}
  virtual ~SendJsonIfSingletonFactory() {}

  virtual SendJsonIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(SendJsonIf* /* handler */) {}

 protected:
  boost::shared_ptr<SendJsonIf> iface_;
};

class SendJsonNull : virtual public SendJsonIf {
 public:
  virtual ~SendJsonNull() {}
  void SendJsonFileName(const std::vector<std::string> & /* filename */) {
    return;
  }
};

typedef struct _SendJson_SendJsonFileName_args__isset {
  _SendJson_SendJsonFileName_args__isset() : filename(false) {}
  bool filename;
} _SendJson_SendJsonFileName_args__isset;

class SendJson_SendJsonFileName_args {
 public:

  SendJson_SendJsonFileName_args() {
  }

  virtual ~SendJson_SendJsonFileName_args() throw() {}

  std::vector<std::string>  filename;

  _SendJson_SendJsonFileName_args__isset __isset;

  void __set_filename(const std::vector<std::string> & val) {
    filename = val;
  }

  bool operator == (const SendJson_SendJsonFileName_args & rhs) const
  {
    if (!(filename == rhs.filename))
      return false;
    return true;
  }
  bool operator != (const SendJson_SendJsonFileName_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const SendJson_SendJsonFileName_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class SendJson_SendJsonFileName_pargs {
 public:


  virtual ~SendJson_SendJsonFileName_pargs() throw() {}

  const std::vector<std::string> * filename;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class SendJson_SendJsonFileName_result {
 public:

  SendJson_SendJsonFileName_result() {
  }

  virtual ~SendJson_SendJsonFileName_result() throw() {}


  bool operator == (const SendJson_SendJsonFileName_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const SendJson_SendJsonFileName_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const SendJson_SendJsonFileName_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class SendJson_SendJsonFileName_presult {
 public:


  virtual ~SendJson_SendJsonFileName_presult() throw() {}


  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class SendJsonClient : virtual public SendJsonIf {
 public:
  SendJsonClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) :
    piprot_(prot),
    poprot_(prot) {
    iprot_ = prot.get();
    oprot_ = prot.get();
  }
  SendJsonClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) :
    piprot_(iprot),
    poprot_(oprot) {
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void SendJsonFileName(const std::vector<std::string> & filename);
  void send_SendJsonFileName(const std::vector<std::string> & filename);
  void recv_SendJsonFileName();
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class SendJsonProcessor : public ::apache::thrift::TProcessor {
 protected:
  boost::shared_ptr<SendJsonIf> iface_;
  virtual bool process_fn(apache::thrift::protocol::TProtocol* iprot, apache::thrift::protocol::TProtocol* oprot, std::string& fname, int32_t seqid, void* callContext);
 private:
  std::map<std::string, void (SendJsonProcessor::*)(int32_t, apache::thrift::protocol::TProtocol*, apache::thrift::protocol::TProtocol*, void*)> processMap_;
  void process_SendJsonFileName(int32_t seqid, apache::thrift::protocol::TProtocol* iprot, apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  SendJsonProcessor(boost::shared_ptr<SendJsonIf> iface) :
    iface_(iface) {
    processMap_["SendJsonFileName"] = &SendJsonProcessor::process_SendJsonFileName;
  }

  virtual bool process(boost::shared_ptr<apache::thrift::protocol::TProtocol> piprot, boost::shared_ptr<apache::thrift::protocol::TProtocol> poprot, void* callContext);
  virtual ~SendJsonProcessor() {}
};

class SendJsonProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  SendJsonProcessorFactory(const ::boost::shared_ptr< SendJsonIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< SendJsonIfFactory > handlerFactory_;
};

class SendJsonMultiface : virtual public SendJsonIf {
 public:
  SendJsonMultiface(std::vector<boost::shared_ptr<SendJsonIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~SendJsonMultiface() {}
 protected:
  std::vector<boost::shared_ptr<SendJsonIf> > ifaces_;
  SendJsonMultiface() {}
  void add(boost::shared_ptr<SendJsonIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void SendJsonFileName(const std::vector<std::string> & filename) {
    size_t sz = ifaces_.size();
    for (size_t i = 0; i < sz; ++i) {
      ifaces_[i]->SendJsonFileName(filename);
    }
  }

};

} // namespace

#endif
