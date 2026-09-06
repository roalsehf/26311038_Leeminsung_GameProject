# DUNGEON DECK 게임 기획서

## 1. 프로젝트 기본 정보

- 학번: 26311038
- 이름: 이민성
- 프로젝트 이름: 26311038_Leeminsung_GameProject
- 게임 제목: DUNGEON DECK
- 장르: 턴제 덱빌딩 카드 RPG
- 플레이 인원: 1명
- 개발 언어: C++17
- 그래픽 라이브러리: glc2d
- 개발 도구: Visual Studio 2022 Community
- 버전 관리: Git, GitHub
- 전체 개발 기간: 4주

## 2. 게임 개요

DUNGEON DECK은 제한된 Energy 안에서 공격, 방어, 보조 카드를 선택해 던전의 적을 쓰러뜨리는 턴제 카드 RPG이다. 플레이어는 적이 다음 턴에 사용할 행동을 미리 확인하고 손패의 카드를 적절한 순서로 사용한다. Stage 1과 Stage 2를 통과해 덱을 강화하고, Stage 3의 Dungeon Warden을 처치하면 게임을 완료한다.

화면 이동과 카드 선택은 키보드로 처리한다. 카드 전투에 집중하기 위해 맵 이동, 장비, 상점, 저장 기능은 제작 범위에서 제외한다.

## 3. 게임 목적과 핵심 재미

게임의 최종 목적은 플레이어 HP가 0이 되기 전에 3개 Stage의 적을 모두 처치하는 것이다.

핵심 재미는 다음과 같다.

- 적 Intent를 확인하고 공격과 방어 중 필요한 행동을 판단한다.
- 한 턴에 사용할 수 있는 Energy 3을 카드 순서에 맞게 배분한다.
- 전투 보상으로 카드를 선택하여 다음 전투에 사용할 덱을 강화한다.
- 최종 점수를 높이기 위해 가능한 많은 HP를 남긴 채 빠르게 전투를 끝낸다.

## 4. 제작 범위와 완성 가능성

남은 기간 안에 완성할 수 있도록 기능을 다음 범위로 제한한다.

- 플레이어 캐릭터 1종
- 적 3종과 Stage 3개
- 기본 카드 Strike, Guard, Focus 3종
- 카드 보상은 기존 카드 3종 중 1장 추가
- 키보드 입력만 사용
- 메인 메뉴, 설명, 전투, 보상, 게임 오버, 게임 클리어 화면
- 로컬 싱글 플레이만 지원

다음 기능은 구현하지 않는다.

- 자유 이동 맵과 충돌 장애물
- 인벤토리와 장비
- 상점과 화폐
- 세이브와 로드
- 온라인 기능
- 복잡한 상태 이상과 카드 강화 트리

## 5. 게임 시작 조건

- 프로그램 실행 후 메인 메뉴가 표시되어야 한다.
- 메인 메뉴에서 GAME START를 선택하고 Enter를 누르면 게임을 시작한다.
- 게임 시작 시 플레이어 HP는 50, Energy는 3, Block은 0으로 초기화한다.
- Strike 5장, Guard 4장, Focus 1장으로 시작 덱을 만든다.
- 시작 덱을 섞은 뒤 Stage 1 Training Goblin 전투를 시작한다.
- 첫 플레이어 턴에 카드 5장을 뽑는다.

## 6. 게임 종료 조건

게임 클리어 조건은 Stage 3의 Dungeon Warden HP를 0 이하로 만드는 것이다. 조건을 만족하면 GAME CLEAR 화면과 최종 점수를 표시한다.

게임 실패 조건은 플레이어 HP가 0 이하가 되는 것이다. 조건을 만족하면 GAME OVER 화면과 획득한 점수를 표시한다.

GAME CLEAR 또는 GAME OVER 화면에서 Enter를 누르면 전투 데이터와 점수를 초기화하고 메인 메뉴로 돌아간다. 메인 메뉴에서 EXIT를 선택하거나 Esc를 누르면 프로그램을 종료한다.

## 7. 조작 방법

### 현재 구현된 조작

- W 또는 위 방향키: 메뉴의 위 항목 선택
- S 또는 아래 방향키: 메뉴의 아래 항목 선택
- Enter: 메뉴 선택, 이전 화면으로 돌아가기
- Esc: 이전 화면으로 돌아가기 또는 프로그램 종료

### 전투 구현 시 사용할 조작

- 숫자 1부터 5: 해당 위치의 손패 카드 사용
- E: 플레이어 턴 종료
- Esc: 전투 중에는 종료 확인, 결과 화면에서는 메인 메뉴 이동

카드 RPG이므로 플레이어 캐릭터의 실시간 방향 이동은 사용하지 않는다. 입력은 메뉴 이동, 카드 선택, 턴 종료에만 사용한다.

## 8. 기본 게임 규칙

- 플레이어 최대 HP는 50이다.
- 플레이어 최대 Energy는 3이다.
- 매 플레이어 턴 시작 시 Energy를 3으로 회복한다.
- 매 플레이어 턴 시작 시 Block을 0으로 초기화한다.
- 기본 손패는 5장이다.
- 카드를 사용하면 카드 비용만큼 Energy가 감소한다.
- Energy가 부족한 카드는 사용할 수 없다.
- 플레이어가 받는 Damage는 Block에서 먼저 차감하고 남은 수치만 HP에서 차감한다.
- 적도 Block을 보유할 수 있으며 Damage는 적 Block에서 먼저 차감한다.
- 플레이어와 적의 HP, Block, Energy는 0보다 작아지지 않도록 처리한다.
- 적의 다음 행동은 Intent로 미리 표시한다.

## 9. 카드와 덱 규칙

시작 덱은 총 10장이다.

- Strike 5장
- Guard 4장
- Focus 1장

카드 정보는 다음과 같다.

### Strike

- 종류: Attack
- 비용: Energy 1
- 효과: 적에게 Damage 6

### Guard

- 종류: Skill
- 비용: Energy 1
- 효과: 플레이어 Block 5 증가

### Focus

- 종류: Skill
- 비용: Energy 0
- 효과: 카드 1장 추가 드로우

덱은 DrawPile, Hand, DiscardPile로 구분한다. 전투 시작 시 덱을 섞어 DrawPile에 넣는다. 카드를 사용하면 Hand에서 제거해 DiscardPile로 이동한다. 턴 종료 시 남은 Hand 카드도 DiscardPile로 이동한다. DrawPile이 부족하면 DiscardPile을 다시 섞어 새로운 DrawPile로 사용한다.

## 10. 점수와 승부 규칙

전투 승리 시 Stage 기본 점수와 남은 HP 보너스를 합산한다.

- Stage 1 클리어 기본 점수: 1000점
- Stage 2 클리어 기본 점수: 1500점
- Stage 3 클리어 기본 점수: 2500점
- 각 Stage 종료 시 남은 HP 1당 보너스: 10점
- 카드 사용 자체에는 점수 증감이 없다.
- 패배하면 그 시점까지 획득한 점수를 GAME OVER 화면에 표시한다.
- Stage 3까지 승리하면 누적 점수를 최종 점수로 표시한다.

승부는 점수와 관계없이 HP로 결정한다. 적 HP가 먼저 0이 되면 해당 전투 승리, 플레이어 HP가 먼저 0이 되면 게임 패배이다.

## 11. 플레이어 행동

- 현재 손패에서 사용할 카드를 선택한다.
- Strike로 적에게 Damage를 준다.
- Guard로 Block을 얻는다.
- Focus로 카드를 추가로 뽑는다.
- 더 사용할 카드가 없거나 전략적으로 턴을 넘길 때 E를 눌러 턴을 종료한다.
- 적의 공격을 받은 뒤 HP가 남아 있으면 다음 플레이어 턴을 시작한다.

Player 객체가 관리할 데이터는 현재 HP, 최대 HP, Energy, 최대 Energy, Block, 누적 점수, 플레이어 텍스처이다. 2주차 코드에는 HP, 최대 HP, Energy, 텍스처와 출력 기능을 우선 반영한다.

## 12. 적, 장애물, 아이템 행동

### Stage 1 Training Goblin

- 최대 HP: 32
- 행동 순서: Attack 7, Defend 5, Heavy Attack 11
- 세 행동을 사용한 뒤 처음부터 반복한다.

### Stage 2 Armored Goblin

- 최대 HP: 45
- 행동 순서: Attack 8, Defend 6, Heavy Attack 12
- 세 행동을 사용한 뒤 처음부터 반복한다.

### Stage 3 Dungeon Warden

- 최대 HP: 65
- 행동 순서: Attack 8, Defend 7, Heavy Attack 14, Attack 8
- 네 행동을 사용한 뒤 처음부터 반복한다.

적은 자신의 턴이 되면 화면에 표시된 Intent와 같은 행동을 실행한다. Defend 행동은 적 Block을 증가시키고 Attack 행동은 플레이어에게 Damage를 준다.

자유 이동 맵을 사용하지 않으므로 물리적인 장애물은 없다. 아이템 역할은 전투 승리 후 받는 카드 보상이 담당한다. Stage 1과 Stage 2 승리 후 Strike, Guard, Focus 중 한 장을 선택하면 다음 Stage부터 덱에 추가된다.

## 13. 게임 화면 구성

### 메인 메뉴 화면

- Main.png 전체 화면 배경
- 배경 이미지에 포함된 DUNGEON DECK 제목
- GAME START, HOW TO PLAY, EXIT 메뉴
- 현재 선택된 메뉴 색상 표시
- 키보드 조작 안내

### 설명 화면

- InGame.png 전체 화면 배경
- 카드 드로우, Energy, 카드 효과, 승리 조건 안내
- Enter 또는 Esc 복귀 안내

### 전투 화면

- InGame.png 전체 화면 배경
- Stage 번호
- 적 이름, HP, Block, 다음 Intent
- Player.png 플레이어 초상화
- 플레이어 HP, Block, Energy, 점수
- 손패 카드 최대 5장과 카드 비용
- 최근 행동 결과 메시지
- END TURN 안내

### 보상 화면

- 전투 승리 문구와 획득 점수
- Strike, Guard, Focus 카드 선택 항목
- 다음 Stage 안내

### 결과 화면

- GAME OVER 또는 GAME CLEAR 문구
- 최종 점수
- Enter 입력 시 메인 메뉴 이동 안내

## 14. 게임 진행 흐름

1. 프로그램을 실행한다.
2. 메인 메뉴에서 GAME START를 선택한다.
3. 플레이어와 시작 덱을 초기화한다.
4. Stage 1 전투를 시작한다.
5. 플레이어가 카드 5장을 뽑고 카드를 사용한다.
6. E를 누르면 적이 Intent에 표시된 행동을 실행한다.
7. 한쪽 HP가 0이 될 때까지 턴을 반복한다.
8. 승리하면 점수를 계산하고 보상 카드를 선택한다.
9. Stage 2와 Stage 3을 같은 흐름으로 진행한다.
10. 플레이어가 사망하면 GAME OVER 화면으로 이동한다.
11. Dungeon Warden을 처치하면 GAME CLEAR 화면으로 이동한다.
12. Enter를 누르면 데이터를 초기화하고 메인 메뉴로 돌아간다.

## 15. 주요 게임 객체 목록

### GameState

- 역할: MainMenu, HowToPlay, Battle, Reward, GameOver, GameClear 상태 구분
- 현재 상태에 따라 입력과 출력 함수를 선택한다.

### TextureResources

- 역할: Main.png, InGame.png, Player.png의 glc2d 텍스처 키 관리
- 프로그램 시작 시 로드하고 종료 전에 해제한다.

### Player

- 데이터: HP, 최대 HP, Energy, 최대 Energy, Block, 점수, 텍스처
- 기능: 초기화, Damage 적용, Block 증가, Energy 사용, 점수 추가, 화면 출력
- 2주차 구현: HP, 최대 HP, Energy, 텍스처 설정과 출력

### Card

- 데이터: 종류, 비용, Damage, Block, DrawCount
- 기능: 카드 데이터 제공과 효과 적용

### Deck

- 데이터: DrawPile, Hand, DiscardPile
- 기능: 초기 덱 생성, 섞기, 드로우, 사용 카드 버리기, 덱 재구성

### Enemy

- 데이터: 이름, HP, 최대 HP, Block, 행동 패턴, 턴 수
- 기능: Intent 계산, 공격, 방어, Damage 적용

### BattleManager

- 역할: 플레이어 턴, 적 턴, 카드 사용, 승패 판정 처리

### StageManager

- 역할: Stage 번호, 적 생성, 다음 Stage 이동, 게임 클리어 판정

### RewardManager

- 역할: 보상 카드 표시와 선택 결과를 플레이어 덱에 추가

## 16. 필요한 이미지 리소스 목록

현재 저장소에 포함하고 화면에 적용한 이미지는 다음과 같다.

- texture/Main.png: 메인 메뉴 전체 화면 배경, 적용 완료
- texture/InGame.png: 설명 화면과 전투 화면 배경, 적용 완료
- texture/Player.png: 전투 화면 플레이어 초상화, 적용 완료

추가로 필요한 이미지는 다음과 같다.

- texture/TrainingGoblin.png: Stage 1 적
- texture/ArmoredGoblin.png: Stage 2 적
- texture/DungeonWarden.png: Stage 3 보스
- texture/CardStrike.png: 공격 카드
- texture/CardGuard.png: 방어 카드
- texture/CardFocus.png: 드로우 카드
- texture/CardFrame.png: 카드 공통 테두리
- texture/ResultPanel.png: 보상과 결과 화면 배경

적과 카드 이미지는 3주차에 준비한다. 현재 제공된 우주 슈팅 리소스는 DUNGEON DECK의 장르와 맞지 않아 게임에 포함하지 않는다.

## 17. 필요한 사운드 리소스 목록

- sound/MainTheme.mp3: 메인 메뉴 배경 음악
- sound/BattleTheme.mp3: 전투 배경 음악
- sound/MenuMove.wav: 메뉴 선택 이동 효과음
- sound/CardSelect.wav: 카드 선택 효과음
- sound/Attack.wav: 공격 카드 효과음
- sound/Guard.wav: 방어 카드 효과음
- sound/EnemyAttack.wav: 적 공격 효과음
- sound/BattleWin.wav: 전투 승리 효과음
- sound/GameOver.wav: 게임 패배 효과음
- sound/GameClear.wav: 최종 승리 효과음

사운드는 목록만 확정하고 실제 파일 준비와 적용은 3주차에 진행한다.

## 18. 리소스 출처와 관리

Main.png, InGame.png, Player.png는 2026년 9월 6일 과제자가 제공한 파일이다. 원본 파일명은 유지하고 프로젝트의 texture 폴더에 저장한다. 제출 전에 이미지의 생성 경로와 사용 권한을 과제자가 최종 확인한다.

Free_Stylized_2D_Space_Shooter_Pack.zip은 폴더 구성과 리소스 종류를 확인하기 위한 참고 자료로만 사용했다. 게임 장르와 맞지 않는 우주선, 미사일, 우주 배경 리소스는 저장소에 추가하지 않았다.

## 19. 2주차 코드 반영 내용

- 기존 메뉴 입력과 GameState 화면 전환 유지
- TextureResources 구조체 추가
- Player 클래스에 HP, 최대 HP, Energy, 텍스처 출력 기능 추가
- 실행 파일 위치를 기준으로 texture 경로 생성
- Main.png, InGame.png, Player.png 로드 성공 여부 확인
- 배경 이미지 화면 크기 조절 출력
- 알파 채널을 사용한 Player.png 출력
- 프로그램 종료 시 텍스처 해제
- Visual Studio 프로젝트에서 texture 파일을 빌드 결과 폴더로 자동 복사

2주차에는 요청된 이미지 삽입과 출력까지만 구현한다. 카드 전투 계산과 적 턴은 3주차에 구현한다.

## 20. 주차별 개발 계획

### 1주차

- Git 저장소와 Visual Studio 프로젝트 구성
- glc2d와 DXSDK 설치 및 복원 구성
- 메인 메뉴, 설명 화면, 전투 미리보기 구현

### 2주차

- 게임 기획서 완성 및 PDF 저장
- 이미지 리소스 목록과 사운드 리소스 목록 확정
- texture 폴더와 텍스처 자동 복사 설정
- 메인 메뉴, 전투 배경, 플레이어 이미지 출력
- Player 핵심 데이터와 출력 기능 코드 반영

### 3주차

- 적과 카드 이미지 및 사운드 준비
- Card, Deck, Enemy, BattleManager 구현
- 카드 사용, Energy, Block, 적 Intent 구현
- 점수, 승패, 보상, Stage 진행 구현

### 4주차

- Dungeon Warden 전투와 결과 화면 완성
- 전체 이미지와 사운드 적용
- 밸런스 조정과 예외 처리
- 전체 빌드와 실행 검증

## 21. 최종 완료 기준

- 메인 메뉴에서 모든 항목을 선택할 수 있다.
- GAME START 선택 시 Stage 1 전투가 시작된다.
- 턴 시작 시 카드 5장을 드로우한다.
- 숫자키로 카드를 사용할 수 있다.
- 카드 비용만큼 Energy가 감소한다.
- 공격, Block, 추가 드로우가 정상 처리된다.
- E로 턴을 종료하면 적 행동이 실행된다.
- 적 처치 후 점수와 보상을 획득한다.
- Stage 2와 Stage 3으로 진행할 수 있다.
- Dungeon Warden 처치 후 GAME CLEAR가 표시된다.
- 플레이어 사망 후 GAME OVER가 표시된다.
- 최종 점수가 결과 화면에 표시된다.
- NuGet 복원 후 프로젝트가 오류 없이 빌드된다.
- 모든 필요한 리소스가 Git 저장소에 포함된다.
- 주차별 Commit과 Tag가 올바른 Commit을 가리킨다.
