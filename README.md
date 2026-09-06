# 26311038_Leeminsung_GameProject

## 과제 정보

- 학번: 26311038
- 이름: 이민성
- 영문 이름: Leeminsung
- 과목명: 게임프로그래밍
- 과제명: C++과 glc2d를 활용한 2D 게임 제작 프로젝트
- 개발 기간: 4주
- 개발 환경: Visual Studio 2022 Community, C++17, glc2d
- 버전 관리: Git, GitHub
- GitHub 저장소: https://github.com/roalsehf/26311038_Leeminsung_GameProject

## 게임 소개

DUNGEON DECK은 제한된 Energy 안에서 공격, 방어, 보조 카드를 선택하여 던전을 공략하는 턴제 덱빌딩 카드 RPG이다. 플레이어는 적이 다음 턴에 사용할 행동을 확인하고, 현재 손패와 남은 Energy를 고려해 카드를 사용한다. 일반 적과 정예 적을 통과한 뒤 최종 보스 Dungeon Warden을 처치하면 게임에서 승리한다.

## 게임 기본 정보

- 게임 제목: DUNGEON DECK
- 장르: 턴제 덱빌딩 카드 RPG
- 플레이 방식: 싱글 플레이
- 핵심 목표: 3개 Stage를 진행하고 최종 보스 Dungeon Warden 처치
- 시작 능력치: HP 50, Energy 3
- 기본 손패: 매 턴 5장
- 시작 덱: Strike 5장, Guard 4장, Focus 1장

## 핵심 재미

- 적의 다음 행동을 먼저 확인하고 공격과 방어의 우선순위를 결정한다.
- 한 턴에 사용할 수 있는 Energy가 제한되어 있어 카드 사용 순서가 중요하다.
- 전투 보상으로 카드를 선택하여 덱을 강화한다.
- Stage가 진행될수록 적의 체력과 공격 방식이 달라져 새로운 판단이 필요하다.

## 게임 진행 흐름

1. 메인 메뉴에서 GAME START를 선택한다.
2. 전투가 시작되면 덱에서 카드 5장을 뽑는다.
3. 적의 다음 행동과 플레이어의 HP, Block, Energy를 확인한다.
4. 손패의 카드를 선택하여 공격하거나 방어한다.
5. END TURN을 선택하면 남은 손패를 버리고 적이 행동한다.
6. 적을 처치하면 카드 보상을 선택하고 다음 Stage로 이동한다.
7. 플레이어 HP가 0이 되면 패배하고, Dungeon Warden을 처치하면 승리한다.

## 카드와 덱 규칙

- DrawPile은 아직 뽑지 않은 카드가 있는 공간이다.
- Hand는 현재 턴에 사용할 수 있는 카드가 있는 공간이다.
- DiscardPile은 사용했거나 턴 종료 때 버린 카드가 있는 공간이다.
- 턴 시작 시 Hand가 5장이 되도록 DrawPile에서 카드를 뽑는다.
- DrawPile이 부족하면 DiscardPile을 섞어 새로운 DrawPile로 사용한다.
- 카드를 사용하면 카드 비용만큼 Energy가 감소한다.
- Energy가 부족한 카드는 사용할 수 없다.
- 턴이 끝나면 남은 손패는 DiscardPile로 이동한다.

## 시작 카드

### Strike

- 종류: 공격 카드
- 비용: Energy 1
- 효과: 적에게 6 Damage
- 시작 수량: 5장

### Guard

- 종류: 방어 카드
- 비용: Energy 1
- 효과: 플레이어에게 5 Block
- 시작 수량: 4장

### Focus

- 종류: 보조 카드
- 비용: Energy 0
- 효과: 카드를 1장 추가로 뽑음
- 시작 수량: 1장

## 전투 규칙

- 플레이어 턴이 시작되면 Energy가 3으로 회복된다.
- 적이 받을 Damage는 적의 HP에서 차감된다.
- 플레이어가 받을 Damage는 Block에서 먼저 차감되고 남은 수치만 HP에서 차감된다.
- Block은 다음 플레이어 턴이 시작될 때 초기화된다.
- 적의 다음 행동은 Intent로 미리 표시한다.
- 적의 HP가 0 이하가 되면 전투에서 승리한다.
- 플레이어의 HP가 0 이하가 되면 게임에서 패배한다.

## Stage 구성

### Stage 1 Training Goblin

- 역할: 기본 전투 규칙 학습
- 특징: 단순한 공격 행동 사용
- 목적: 카드 선택과 Block 사용 방법 익히기

### Stage 2 Armored Goblin

- 역할: 정예 적 전투
- 특징: 공격과 방어 행동을 번갈아 사용
- 목적: 적 Intent에 맞춘 공격과 방어 판단 강화

### Stage 3 Dungeon Warden

- 역할: 최종 보스
- 특징: 강한 공격과 특수 행동 사용
- 목적: 강화한 덱을 활용하여 최종 전투 완료

## 화면 구성

- MAIN MENU: GAME START, HOW TO PLAY, EXIT 항목 표시
- HOW TO PLAY: 기본 조작과 전투 규칙 안내
- BATTLE: 플레이어 정보, 적 정보, 적 Intent, 손패, Energy, 턴 종료 버튼 표시
- REWARD: 전투 완료 후 획득할 카드 선택
- GAME OVER: 패배 결과와 메인 메뉴 이동 항목 표시
- GAME CLEAR: 최종 보스 처치 결과와 메인 메뉴 이동 항목 표시

## 현재 구현 내용

- glc2d를 이용한 게임 창과 메인 루프 생성
- 글꼴 출력과 키보드 입력 처리
- Main.png를 사용한 메인 메뉴 배경 출력
- InGame.png를 사용한 설명 및 전투 배경 출력
- Player.png를 사용한 플레이어 초상화 출력
- 실행 파일 위치를 기준으로 한 텍스처 경로 처리
- 빌드 결과 폴더로 texture 파일을 자동 복사하는 프로젝트 설정
- GAME START, HOW TO PLAY, EXIT 메인 메뉴 구현
- W, S, 방향키를 사용한 메뉴 이동 구현
- Enter 선택과 Esc 이전 화면 또는 종료 기능 구현
- 기본 규칙을 확인할 수 있는 HOW TO PLAY 화면 구현
- 플레이어와 Training Goblin 정보를 표시하는 전투 미리보기 구현
- 메뉴, 설명 화면, 전투 미리보기를 GameState로 구분
- Player 클래스에 HP, 최대 HP, Energy, 텍스처 출력 기능 구현

현재 GAME START는 이미지가 적용된 전투 미리보기 화면으로 연결된다. 2주차에는 기획서 완성 및 텍스처 삽입과 출력까지만 구현하며, 실제 카드 드로우, 카드 선택, Damage, Block, 적 턴, 보상 처리는 이후 주차에 순차적으로 구현한다.

## 주차별 구현 계획

### 1주차

- Visual Studio C++ 프로젝트와 glc2d 환경 구성
- Git 저장소, README, .gitignore 구성
- 메인 메뉴, 설명 화면, 전투 미리보기 구현

### 2주차

- 게임 기획서 완성
- 메인 메뉴와 전투 화면 이미지 리소스 적용
- Player 핵심 데이터와 텍스처 출력 기능 구현
- glc2d 텍스처 로드, 크기 조절, 알파 출력, 해제 구현

### 3주차

- 이미지와 사운드 리소스 준비
- Deck, Battle, Reward, Stage 시스템 구현
- 실제 카드 전투와 Stage 진행 구현

### 4주차

- 최종 보스와 게임 클리어 구현
- 그래픽과 사운드 적용
- 밸런스 조정, 예외 처리, 전체 테스트

## 프로젝트 구성

- 26311038_Leeminsung_GameProject/26311038_Leeminsung_GameProject.sln: Visual Studio 솔루션
- 26311038_Leeminsung_GameProject/26311038_Leeminsung_GameProject.vcxproj: C++ 프로젝트
- 26311038_Leeminsung_GameProject/main.cpp: 게임 소스 코드
- 26311038_Leeminsung_GameProject/packages.config: NuGet 패키지 정보
- 26311038_Leeminsung_GameProject/texture/Main.png: 메인 메뉴 배경
- 26311038_Leeminsung_GameProject/texture/InGame.png: 전투 화면 배경
- 26311038_Leeminsung_GameProject/texture/Player.png: 플레이어 초상화
- docs/GAME_DESIGN.md: 전체 게임 기획서
- docs/GameDesign.pdf: 2주차 제출용 게임 기획서
- .gitignore: Git 제외 파일 규칙

## 준비 자료

- Visual Studio 2022 Community: https://aka.ms/vs/17/release/vs_community.exe
- Microsoft.DXSDK.D3DX 9.29.952.8: https://www.nuget.org/packages/Microsoft.DXSDK.D3DX/9.29.952.8
- glc2d NuGet 패키지: https://www.nuget.org/packages/glc2d
- glc2d 라이브러리 소스: https://github.com/3dapi/glc2d_lib

## 실행 방법

1. 26311038_Leeminsung_GameProject 폴더의 26311038_Leeminsung_GameProject.sln을 Visual Studio 2022로 연다.
2. 솔루션을 우클릭하고 NuGet 패키지 복원을 선택한다.
3. 구성을 Debug, 플랫폼을 x64로 선택한다.
4. Ctrl+Shift+B를 눌러 빌드한다.
5. Ctrl+F5를 눌러 실행한다.

## 현재 조작 방법

- 메뉴 이동: W, S 또는 위, 아래 방향키
- 선택: Enter
- 이전 화면 또는 종료: Esc

## 1주차 제출 정보

- 저장소 이름: 26311038_Leeminsung_GameProject
- 프로젝트 이름: 26311038_Leeminsung_GameProject
- Git Tag: 26311038_Leeminsung_GameProject_w01

## 2주차 제출 정보

- 완성된 게임 기획서: docs/GameDesign.pdf
- 현재 C++ 소스 코드와 Visual Studio 프로젝트 전체 포함
- 이미지 리소스: 26311038_Leeminsung_GameProject/texture
- Git Tag: w02
