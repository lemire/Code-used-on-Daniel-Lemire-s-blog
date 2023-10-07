import options, asyncdispatch

import httpbeast

proc onRequest(req: Request): Future[void] =
  if req.httpMethod == some(HttpGet):
    case req.path.get()
    of "/simple":
      req.send("Hello World")
    else:
      req.send(Http404)

run(onRequest, initSettings(port=Port(3000)))
