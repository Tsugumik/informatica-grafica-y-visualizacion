### Oto zawartość pliku explain.md

Poniżej wklejam kompletną treść, którą możesz zapisać jako `explain.md` w katalogu głównym projektu.

---

### Cel projektu w skrócie
To mała aplikacja C++14 korzystająca z OpenGL i GLUT (FreeGLUT), która otwiera interaktywne okno 3D. Projekt zawiera:
- prosty model sceny 3D (sześcian + dwie „rury”/walce oraz opcjonalne osie),
- kamerę z obsługą rzutowania równoległego i perspektywicznego,
- interfejs/obsługę klawiatury i podziału okna na 4 widoki,
- standardową konfigurację CMake (z logiką zależną od platformy) i metadane Conan.


### Struktura plików (co jest czym)
- `src/pr2b.cpp` — punkt wejścia aplikacji:
    - Tworzy globalny obiekt `cgvInterface` i w `main` wywołuje kolejno:
        - `configure_environment(...)` — inicjalizacja GLUT/okna i ustawień OpenGL,
        - `initialize_callbacks()` — rejestracja callbacków (klawiatura, resize, render),
        - `start_display_loop()` — uruchomienie pętli zdarzeń GLUT (`glutMainLoop`).

- `src/cgvInterface.h/.cpp` — warstwa interfejsu do GLUT i „scalacz” sceny + kamery:
    - Przechowuje stan okna (`window_width`, `window_height`), flagę `windowChange` (tryb 1 widoku vs. podział na 4) oraz obiekty `cgvScene3D scene` i `cgvCamera camera`.
    - `configure_environment(...)`:
        - Inicjuje GLUT (`glutInit...`), tworzy okno, włącza `GL_DEPTH_TEST`, `GL_LIGHTING`, `GL_NORMALIZE`, ustawia kolor tła.
        - Woła `create_world()` do ustawienia początkowych parametrów kamery.
    - `create_world()`:
        - Ustawia wektory kamery: `p0` (pozycja), `r` (punkt referencyjny), `V` (wektor „up”).
        - Konfiguruje kamerę jako równoległą (`CGV_PARALLEL`) z pudełkiem widoku i zasięgiem `znear/zfar`.
        - Ustawia też domyślne wartości dla perspektywy (`angle`, `aspect`).
    - `initialize_callbacks()` rejestruje statyczne funkcje callbacków GLUT:
        - `set_glutKeyboardFunc` — obsługa klawiatury,
        - `set_glutReshapeFunc` — reagowanie na zmianę rozmiaru okna,
        - `set_glutDisplayFunc` — renderowanie sceny.
    - `set_glutDisplayFunc()`:
        - Czyści bufory (`glClear`).
        - Gdy `windowChange == false`: jeden viewport na całe okno i rysowanie sceny (`scene.display()`).
        - Gdy `windowChange == true`: dzieli okno na 4 viewports (ćwiartki) i dla każdej ustawia kamerę inną metodą (`update_camera_view(...)`), po czym rysuje scenę czterokrotnie.
        - Swap buforów (`glutSwapBuffers`).
    - `set_glutReshapeFunc(w, h)`:
        - Aktualizuje rozmiar okna w obiekcie i ponownie stosuje ustawienia kamery (`camera.apply()`), co odświeża macierze projekcji/widoku.
    - `set_glutKeyboardFunc(key, x, y)` — ważne skróty klawiszowe:
        - `p`/`P`: przełącza rzutowanie `CGV_PARALLEL` ↔ `CGV_PERSPECTIVE` i woła `camera.apply()`.
        - `v`/`V`: przełącza predefiniowane pozycje widoku (podłoga, front, profil, bazowy) poprzez `update_camera_view(...)`.
        - `+`/`-`: zoom kamery (`camera.zoom(...)`) i `camera.apply()`.
        - `n`/`N`: zwiększa/zmniejsza `znear` i stosuje kamerę.
        - `4`: przełącza podział okna na 4 widoki (`windowChange`).
        - `e`: włącza/wyłącza rysowanie osi w scenie (`scene.set_ejes(...)`).
        - `Esc`: kończy program (`exit(1)`).
    - `update_camera_view(int pos)` — ustawia kamerę na jedną z 4 pozycji:
        - 1: „bazowy” (`p0, r, V`), 2: widok z góry (podłoga), 3: z przodu (front), 4: z boku (profil). Następnie `camera.apply()`.

- `src/cgvCamera.h/.cpp` — implementacja kamery i projekcji:
    - Obsługuje typy rzutowania: `CGV_PARALLEL`, `CGV_PERSPECTIVE`, `CGV_FRUSTRUM`.
    - Atrybuty: dla równoległej/frustum (`xwmin/xwmax/ywmin/ywmax`), dla perspektywicznej (`angle`, `aspect`), dla wszystkich (`znear`, `zfar`), parametry widoku (`P0`, `r`, `V`).
    - `apply()` ustawia macierz projekcji:
        - `glOrtho(...)` dla równoległej,
        - `glFrustum(...)` dla frustum,
        - `gluPerspective(...)` dla perspektywy.
        - Następnie ustawia macierz modelu-widoku przez `gluLookAt(P0, r, V)`.
    - `zoom(double factor)`: dla rzutów równoległych/frustum skaluje okno widoku (`xwmin...`) — efekt zoomu, dla perspektywy skaluje `angle` (z ograniczeniem < 180°).
    - `set(...)` — przeciążone, do konfiguracji równoległej/frustum i perspektywicznej.

- `src/cgvScene3D.h/.cpp` — definicja sceny i jej rysowania:
    - Flaga `axis` steruje rysowaniem osi pomocniczych (domyślnie `true`).
    - `display()` rysuje scenę:
        - Ustawia światło punktowe `GL_LIGHT0` (pozycja `[10, 8, 9, 1]`) i je włącza.
        - `glPushMatrix()` zachowuje macierz modelowania.
        - Gdy `axis == true`, woła `paint_axes()`:
            - Rysuje trzy linie w kolorach RGB wzdłuż osi X/Y/Z, bardzo długie, aby były widoczne niezależnie od skali.
        - Ustawia kolor emisyjny materiału dla „bryły” sceny (`cube_color`).
        - Rysuje skalowany sześcian (`glScalef(1,2,4); glutSolidCube(1);`).
        - Dwa obiekty „rur” (walce) poprzez pomocnicze `paint_tube()`:
            - `paint_tube()` tworzy `GLUquadric`, przesuwa wzdłuż Z (`glTranslatef(0,0,-0.5)`), rysuje cylinder `gluCylinder(pipe, 0.25, 0.25, 1, 20, 20)`, usuwa quadric.
            - W `display()` każda rura jest dodatkowo obracana `glRotatef(±45, 1,0,0)` i skalowana w Z (`glScalef(1,1,4.5)`) tworząc kształt przypominający skosy/oporę.
        - Na końcu `glPopMatrix()` przywraca macierz.

- `src/cgvPoint.h/.cpp` — proste klasy punktów/wektorów:
    - `cgvPoint3D`: przechowuje `x,y,z` w `std::array<float,3>`, operatory `==`, `!=`, indeksowanie `operator[]`, `set(...)`, `data()`.
    - `cgvPoint4D`: analogicznie dla `x,y,z,w` (`std::array<float,4>`).
    - Definiuje też enum indeksów `X,Y,Z,W` ułatwiający czytanie kodu.

- `CMakeLists.txt` — konfiguracja budowania:
    - Tworzy wykonywalny `pr2b` z plików w `src/`.
    - Logika zależna od platformy:
        - `APPLE`: linkuje frameworki `OpenGL` i `GLUT`, dodaje `GL_SILENCE_DEPRECATION`.
        - `LINUX`: wyszukuje `OpenGL` i `GLUT` przez `find_package` i linkuje biblioteki.
        - `WIN32`: oczekuje pakietów z Conana (`opengl/system`, `opengl-registry`, `freeglut`) i linkuje odpowiednie cele.
        - Na końcu dla nie-`APPLE` linkuje `OpenGL::GL` (jeśli dostępny z `find_package`/toolchaina).

- `README.md` — dokumentacja projektu:
    - Opisuje stos technologiczny, układ plików, sposób budowania (CMake/Conan oraz CLion), wymagania i wskazówki dla poszczególnych platform.
    - Wymienione cele CMake: `pr2b` (aplikacja).

- `conanfile.py`/`conandata.yml` — metadane Conana (wymagania: `opengl-registry`, `opengl/system`, `freeglut`) — pomocne zwłaszcza na Windows/Linux. Na macOS często wystarczą systemowe frameworki.


### Przepływ działania aplikacji (od startu do renderingu)
1) `main` w `pr2b.cpp` woła `cgvInterface.configure_environment(...)`:
    - GLUT tworzy okno, włącza się bufor Z, oświetlenie i normalizacja wektorów normalnych.
    - `create_world()` ustawia parametry i typ kamery.
2) `initialize_callbacks()` rejestruje 3 funkcje:
    - klawiatura → `set_glutKeyboardFunc`,
    - zmiana rozmiaru → `set_glutReshapeFunc`,
    - rysowanie → `set_glutDisplayFunc`.
3) `start_display_loop()` uruchamia pętlę GLUT. Od teraz:
    - przy każdym potrzebnym odświeżeniu wywoła się `set_glutDisplayFunc()` → ustawienie viewportów → `scene.display()` → `glutSwapBuffers()`.
    - klawisze modyfikują stan kamery/sceny i wywołują `glutPostRedisplay()`, aby przerysować.


### Sterowanie z klawiatury (podsumowanie)
- `p`/`P` — przełącz rzutowanie równoległe/perspektywiczne.
- `v`/`V` — zmień ustawienie kamery (bazowe → podłoga → przód → profil).
- `+`/`-` — zoom (skalowanie objętości widoku dla rzutów równoległych lub zmiana kąta `fov` dla perspektywy).
- `n`/`N` — odpowiednio zwiększ/zmniejsz `znear`.
- `4` — przełącz tryb 1 widoku ↔ 4 widoków (ćwiartki okna).
- `e` — włącz/wyłącz rysowanie osi XYZ.
- `Esc` — wyjście z programu.


### Uwagi o OpenGL w projekcie
- To klasyczny, stałofunkcyjny pipeline OpenGL (`glMatrixMode`, `glLoadIdentity`, `glOrtho/glFrustum/gluPerspective`, `gluLookAt`, immediate mode `glBegin/glEnd`).
- Oświetlenie włączone (`GL_LIGHTING`) oraz używane są proste kolory emisyjne materiałów (`glMaterialfv(..., GL_EMISSION, ...)`).
- Dla walców używane są obiekty GLU (`GLUquadricObj`, `gluCylinder`).


### Jak zbudować/uruchomić (w CLion)
- Projekt zawiera profil CLion „Debug” z katalogiem budowy `cmake-build-debug` i celem `pr2b`.
- W CLion: wybierz konfigurację `pr2b`, zbuduj i uruchom. Program otworzy okno z prostą sceną 3D.


### Gdzie dopisywać własny kod
- Nowe obiekty sceny: najprościej w `cgvScene3D::display()` (twórz własne funkcje pomocnicze na wzór `paint_tube()` i używaj `glPushMatrix()/glPopMatrix()`).
- Nowe skróty klawiaturowe lub zachowania okna: w `cgvInterface::set_glutKeyboardFunc` i `set_glutDisplayFunc`.
- Nowe ustawienia kamery/widoków: w `cgvInterface::update_camera_view` (dodaj nowe przypadki) oraz w `cgvCamera`.


### Najważniejsze zależności/platformy
- macOS: linkowanie do frameworków `OpenGL` i `GLUT` (CMake robi to automatycznie, definiuje też `GL_SILENCE_DEPRECATION`).
- Linux: `find_package(OpenGL)` i `find_package(GLUT)`; alternatywnie Conan może dostarczyć FreeGLUT.
- Windows: Conan dostarcza `opengl/system`, `opengl-registry`, `freeglut`; CMake linkuje odpowiednie targety.

---

### Jak zapisać ten plik
- Ręcznie: utwórz plik `explain.md` w katalogu głównym repozytorium i wklej powyższą treść.
- Albo użyj IDE (np. CLion): New File → `explain.md` → wklej zawartość → zapisz.

Jeśli wolisz, mogę dopasować treść `explain.md` do innego formatu (np. krótsze podsumowanie, angielska wersja, albo więcej sekcji o budowaniu/uruchamianiu).