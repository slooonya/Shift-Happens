<a id="readme-top"></a>

<div align="center">
  <img width="1680" height="200" alt="Banner" src="https://github.com/user-attachments/assets/fecdfd65-39ff-4e9b-bcae-fa4296367ceb" />
</div>

<br>
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About the Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>

## About the Project
**Shift Happens** is an arcade game that challenges the player to navigate an obstacle course set in the streets of a megapolis. Dodge traffic cones, boulders, potholes, cars, and other hazards along the way, and try to do it while your old bucket of a car, with its wrecked brakes, keeps accelerating to add more funsies to your day! Put on a show for the clueless onlookers on the sidewalks! If this doesn't prepare you for every driving contingency possible, I don't know what will!

<img width="1409" height="1413" alt="475537720-2764d4ca-8ed1-414e-afc9-1df0bfdd7823" src="https://github.com/user-attachments/assets/954c4b92-aa12-4e8e-aeb6-b1069e2edd57" />

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Built With
[![C++][C++]][C++-url]
[![OpenGL][OpenGL]][OpenGL-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Getting Started

### Prerequisites
#### 1. [Visual Studio](https://visualstudio.microsoft.com/vs/community/)

#### 2. [freeglut](https://freeglut.sourceforge.net/index.php#download)

#### 3. [Git](https://git-scm.com/)

### Installation
#### 1. Clone the repository
```bash
git clone https://github.com/slooonya/Shift-Happens.git
```

#### 2. Navigate to the project directory
```bash
cd Shift-Happens
```

#### 3. Create a project
  - Click Create a new project
  - Select Empty Project (C++)
  - Add assets and the .cpp from the cloned repository to the Source Files

#### 4. Set up freeglut
  - Go to solution properties
  - Click the symbol ‘v’ beside the ‘Platform’, choose ‘x64’
  - Click ‘VC++ Directories’ on the left side, then the symbol ‘v’ which is on the right side of ‘Include Directories’, and choose the ‘<Edit...>’ in the list. Press the ‘New Line’ button and choose the ‘include’ folder of the freeglut zip downloaded. Click the ‘OK’ button, to go back to the previous page.
  - Click symbol ‘v’ on the right side of ‘Library Directories’ (blue arrow), and choose ‘<Edit...>’ in the list. Press the ‘New Line’ button, and choose the ‘lib’ folder of the freeglut zip downloaded. Click the ‘OK’ button, to go back to the previous page.
  - Click the symbol ‘>’ which is on the left side of ‘Linker’, and choose ‘Input’ in the list of ‘Linker’. Add the text "freeglut_static.lib;" into ‘Additional Dependencies’
  - Click the ‘>’ symbol which is on the left side of ‘Manifest Tool’, and choose ‘Input and Output’ in the list of ‘Manifest Tool’; Set ’Embed Manifest’ to ‘No’; Click ‘OK’.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Contributing

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## Contact

Sonya's email address: snmmnva@gmail.com

Project Link: https://github.com/slooonya/Shift-Happens

<img width="1680" height="200" alt="475537646-fcac2301-9e01-4f1f-b6a2-3c49c0fe8ff6" src="https://github.com/user-attachments/assets/8f298787-fea7-4cba-a432-8471da4090c9" />

<p align="right">(<a href="#readme-top">back to top</a>)</p>

[OpenGL]: https://img.shields.io/badge/OpenGL-%23FFFFFF.svg?style=for-the-badge&logo=opengl
[OpenGL-url]: https://www.khronos.org/opengl/

[C++]: https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white
[C++-url]: https://learn.microsoft.com/en-us/cpp/cpp/?view=msvc-170
