#include "SDLGraphicsProgram.hpp"


// Constructor: Initializes the SDL graphics program
// @param w: Window width
// @param h: Window height
SDLGraphicsProgram::SDLGraphicsProgram(int w, int h){
	// Initialization flag
	bool success = true;
	// String to hold any errors that occur.
	std::stringstream errorStream;
	// Initialize the window pointer
	m_window = NULL;

	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO)< 0){
		errorStream << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
		success = false;
	}
	else{
		//Use OpenGL 3.3 core
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
		// request a double buffer for smooth updating.
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		//Create window
		m_window = SDL_CreateWindow( "Lab",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                w,
                                h,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

		// Check if Window created.
		if( m_window == NULL ){
			errorStream << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		//Create an OpenGL Graphics Context
		m_openGLContext = SDL_GL_CreateContext( m_window );
		if( m_openGLContext == NULL){
			errorStream << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		// Initialize GLAD Library
		if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){
			errorStream << "Failed to iniitalize GLAD\n";
			success = false;
		}

		//Initialize OpenGL
		if(!InitGL()){
			errorStream << "Unable to initialize OpenGL!\n";
			success = false;
		}
  	}

    // If initialization did not work, then print out a list of errors in the constructor.
    if(!success){
        errorStream << "SDLGraphicsProgram::SDLGraphicsProgram - Failed to initialize!\n";
        std::string errors=errorStream.str();
        SDL_Log("%s\n",errors.c_str());
    }else{
        SDL_Log("SDLGraphicsProgram::SDLGraphicsProgram - No SDL, GLAD, or OpenGL, errors detected during initialization\n\n");
    }

	// SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN); // debug
	GetOpenGLVersionInfo();


    // Setup Renderer
    m_renderer = new Renderer(w,h);
    // Set start time
    Uint32 startTime = SDL_GetTicks();
    m_renderer->SetStartTime(startTime);    
}


// Destructor: Cleans up resources and shuts down SDL
SDLGraphicsProgram::~SDLGraphicsProgram(){
    if(m_renderer!=nullptr){
        delete m_renderer;
    }


    // Destroy the SDL window and quit SDL subsystems
	SDL_DestroyWindow( m_window );
	m_window = nullptr;
	SDL_Quit();
}


// Initialize OpenGL
bool SDLGraphicsProgram::InitGL(){
	//Success flag
	bool success = true;
	return success;
}

// Processes user input, including keyboard and mouse events
// @param quit: Flag to indicate whether the program should quit
// @param cameraSpeed: Speed at which the camera moves
void SDLGraphicsProgram::Input(bool& quit, float cameraSpeed) {
    SDL_Event e;
    //Handle events on queue
    while(SDL_PollEvent( &e ) != 0){
        // User posts an event to quit
        if(e.type == SDL_QUIT){
            quit = true;
        }
        // Handle keyboard input for the camera class
        if(e.type==SDL_MOUSEMOTION){
            // Handle mouse movements
            int mouseX = e.motion.x;
            int mouseY = e.motion.y;
            // m_renderer->SetMousePosition(mouseX, mouseY);
            m_renderer->GetCamera(0)->MouseLook(mouseX, mouseY);
        }
        switch(e.type){
            // Handle keyboard presses
            case SDL_KEYDOWN:
                switch(e.key.keysym.sym){
                    case SDLK_LEFT:
                        m_renderer->GetCamera(0)->MoveLeft(cameraSpeed);
                        break;
                    case SDLK_RIGHT:
                        m_renderer->GetCamera(0)->MoveRight(cameraSpeed);
                        break;
                    case SDLK_UP:
                        m_renderer->GetCamera(0)->MoveForward(cameraSpeed);
                        break;
                    case SDLK_DOWN:
                        m_renderer->GetCamera(0)->MoveBackward(cameraSpeed);
                        break;
                    case SDLK_RSHIFT:
                        m_renderer->GetCamera(0)->MoveUp(cameraSpeed);
                        break;
                    case SDLK_RCTRL:
                        m_renderer->GetCamera(0)->MoveDown(cameraSpeed);
                        break;
                }
            break;
        }
    }
}

Object* skybox;
SkyboxNode* skyboxNode;

// Initializes the scene graph
void SDLGraphicsProgram::InitSceneGraph() {
    skybox = new Object();
    skyboxNode = new SkyboxNode(skybox);
    skyboxNode->Init(skybox);
    m_renderer->setRoot(skyboxNode);
    std::cout << "Scene Graph Initialized" << std::endl;
}

// Updates objects in the scene
void SDLGraphicsProgram::UpdateObjectsInScene() {

}

// Main program loop
void SDLGraphicsProgram::Loop(){
    std::cout << "Starting loop" << std::endl;
    // Set a default position for our camera
    // m_renderer->GetCamera(0)->SetCameraEyePosition(0.0f,0.0f,20.0f);
    
    // Main loop flag
    bool quit = false;
    // Enable text input
    SDL_StartTextInput();
    // Set the camera speed for how fast we move.
    float cameraSpeed = 5.0f;
    // Set a default position for our camera
    m_renderer->GetCamera(0)->SetCameraEyePosition(0.0f,0.0f,100.0f);
    InitSceneGraph();

    while(!quit){
        Input(quit, cameraSpeed);
        UpdateObjectsInScene();
        
        // Update our scene through our renderer
        m_renderer->Update();

        // Render our scene using our selected renderer
        m_renderer->Render();

        // Add delay to control frame rate
        SDL_Delay(25);

      	//Update screen of our specified window
      	SDL_GL_SwapWindow(GetSDLWindow());
	}
    //Disable text input
    SDL_StopTextInput();
}


// Get Pointer to Window
SDL_Window* SDLGraphicsProgram::GetSDLWindow(){
  return m_window;
}

// Helper Function to get OpenGL Version Information
void SDLGraphicsProgram::GetOpenGLVersionInfo(){
	SDL_Log("(Note: If you have two GPU's, make sure the correct one is selected)");
	SDL_Log("Vendor: %s",(const char*)glGetString(GL_VENDOR));
	SDL_Log("Renderer: %s",(const char*)glGetString(GL_RENDERER));
	SDL_Log("Version: %s",(const char*)glGetString(GL_VERSION));
	SDL_Log("Shading language: %s",(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}
