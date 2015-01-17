#ifndef _LOFTILI_SERVER_H
#define _LOFTILI_SERVER_H

#include <microhttpd.h>
#include <ao/ao.h>
#include "loftili.h"
#include "dispatch/dispatch.h"
#include "dispatch/router.h"
#include "services/registration.h"
#include "communication/request.h"
#include "communication/response.h"

namespace loftili {

class Server {

  // MEMBER
  public:
    Server();
    ~Server();
    int process(struct ahc_info info);

  private:
    Dispatch dispatch;
    Router router;

  // STATIC
  public:
    static int run(Configuration config);

    static int ahc(
      void* cls, 
      MHD_Connection* connection, 
      const char* url, 
      const char* method,
      const char* version,
      const char* upload_data,
      size_t* data_size,
      void** con_cls
    );

    static void completion(
      void *cls,
      struct MHD_Connection *connection,
      void **con_cls,
      enum MHD_RequestTerminationCode toe
    );

  private:
    static Server* server_instance;

};

}

#endif
