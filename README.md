# chat-in-c
<hr>
<h1>프로젝트 멤버 및 역할</h1>
<hr>
<h1>프로젝트 소개 및 개발 내용 소개</h1>
이 프로젝트는 터미널 환경에서의 채팅 프로그램 입니다.<br>
파일을 서버로 올리고 다른 클라이언트가 서버에 있는 파일을 내려 받을 수 있습니다.<br>
오픈소스 인 imcat 을 이용해서 터미널 환경에서 이미지 파일을 볼 수 있습니다.<br>
<h1>프로젝트 개발 결과 소개</h1>
#server.c
<ul>
  <li>send_msg()-모든 클라이언트 들에게 메세지를 전송하는 함수 명령어를 확인하고 처리한다.</li>
  <li>handle_cli()-클라이언트로 부터 메세지를 받고 클라이언트가 종료하면 소켓을 지운다</li>
  <li>client_cnt-클라이언트의 갯수 변수</li>
  <li>client_sockets-클라이언트의 소켓번호를 저장하는 배열</li>
  </ul>
 #client.c
 <ul>
  <li>ID-클라이언트의 닉네임 변수</li>
  <li>msg-클라이언트가 보내는 메세지</li>
  <li>send_msg()-서버로 메세지를 전송한다</li>
  <li>recv_msg()-서버로 부터 메세지를 받는다.</li>
  </ul>
  ![dia](https://user-images.githubusercontent.com/54802110/101599516-3ade5f00-3a3d-11eb-85dd-b56aedb411a0.JPG)
  
  /send [fileanme]- 클라이언트가 가지고 있는 파일을 서버로 전송한다.<br>
  /down [filename]-서버가 가지고있는 파일을 클라이언트로 전송 한다.<br>
  /view [filename]-클라이언트가 가지고있는 이미지 파일을 출력한다.<br>
  
  <h1>개발결과물 사용방법</h1>
  
  <img src="./images/스크린샷1.png" width="500"></img><br>
  서버를 실행시킨후 클라이언트를 실행시키고 닉네임을 입력한다.<br>
  <img src="./images/스크린샷2.png" width="500"></img><br>
  대화를 나누는 장면<br>
   <img src="./images/스크린샷3.png" width="500"></img><br>
   /send [filename] 명령을 이용해 폴더에있는 apple.jpg를 서버로 전송한다.<br>
    <img src="./images/스크린샷4.png" width="500"></img><br>
    /down [filename] 명령을 이용해 서버로 부터 apple.jpg를 다운 받는다.<br>
     <img src="./images/스크린샷5.png" width="500"></img><br>
     /view [filename] 명령을 이용해 내폴더에 있는 사진을 3초간 띄운다.<br>
  
<h1>결과물의 필요성 및 활용방안</h1>
터미널 환경에서 작동 되기 때문에 가볍다. 꼭 터미널 환경을 사용해야하는 낮은 성능의 기기에서도 사용 할 수 있다.
