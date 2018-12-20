import http.server
import socketserver
import os
import urllib.request
import time
import json
from _operator import concat


class Handler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        # По какому пути к нам обратился пользователь
        print(self.path)
        if(self.path=="/tst1"):
          self.send_response(200)
          self.send_header('Content-type', 'application/json')
          self.end_headers()
          self.wfile.write(b'{\'hello\':1234}')
          return
        if(self.path=="/tst2"):
          self.send_response(200)
          self.send_header('Content-type', 'application/json')
          self.end_headers()
          self.wfile.write(b'{\'bye\':4321}')
          return
        if(self.path=="/auth/getNonce"):
          self.send_response(200)
          self.send_header('Content-type', 'application/json')
          self.end_headers()
          self.wfile.write(b'{\"value\":\"12345\",\"status\":\"ok\"}')
          return

    def do_POST(self):
        print("got post!! ",self.path)
        if (self.path == "/auth/getToken"):
          self.data_string = self.rfile.read(int(self.headers['Content-Length']))
          print(self.data_string)
          self.send_response(200)
          self.send_header('Content-type', 'application/json')
          self.end_headers()
          self.wfile.write(b'{\"token\":\"12345\",\"status\":\"ok\"}')
          return
        if (self.path == "/processing/getItemsToRate"):
          self.data_string = self.rfile.read(int(self.headers['Content-Length']))
          print(self.data_string)
          self.send_response(200)
          self.send_header('Content-type', 'application/json')
          self.end_headers()
          self.wfile.write(b'{\"items\":[\"1\",\"2\",\"3\",\"4\",\"5\",\"6\",\"7\"]}')
          return
        if (self.path == "/processing/getListWithRecom"):
          self.data_string = self.rfile.read(int(self.headers['Content-Length']))
          print(self.data_string)
          self.send_response(200)
          self.send_header('Content-type', 'application/json')
          self.end_headers()
          self.wfile.write(b'{\"items\":[\"1\",\"2\",\"3\",\"4\",\"5\",\"6\",\"7\"]}')
          return
        if (self.path == "/content/getItemInfo"):
          self.data_string = self.rfile.read(int(self.headers['Content-Length']))
          jsonstr = (self.data_string).decode("utf-8")
          print("json string = " + jsonstr)
          itemjs = json.loads(jsonstr)
          itemid = itemjs["itemId"]
          self.send_response(200)
          self.send_header('Content-type', 'application/json')
          self.end_headers()
          self.wfile.write(str.encode('{\"itemId\":\"'+str(itemid)+'\",\"name\":\"Ololol\",\"poster\":\"file:///h:/HomeDevelopment/PyProjects/RecomServer/posters/'+str(itemid)+'.jpg\"}'))
          return


httpd = socketserver.TCPServer(('', 8080), Handler)
httpd.serve_forever()
