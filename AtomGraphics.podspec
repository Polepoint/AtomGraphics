Pod::Spec.new do |s|
  s.name             = 'AtomGraphics'
  s.version          = '0.0.1'
  s.summary          = 'AtomGraphics'

  s.description      = <<-DESC
                       This is a Cross-Platform Graphics SDK project
                       DESC

  s.homepage         = 'https://code.fineres.com/projects/MO/repos/atom-graphics/browse/AtomGraphics'
  s.license          = 'MIT'
  s.author           = { 'neo' => 'neo@fanruan.com' }
  s.source           = { :git => 'ssh://git@code.fineres.com:7999/MO/atomgraphics.git', :tag => s.version }

  s.platform = :ios
  s.ios.deployment_target = '8.2'

  s.source_files = "AtomGraphics/**/*.{h,m,mm,cpp,inl}"
  s.exclude_files = "AtomGraphics/**/android/**/**",
                     "AtomGraphics/**/v8/**/**",
                     "AtomGraphics/gpu/**/**",
                     "AtomGraphics/ipc/**/**",
                     "AtomGraphics/third_party/**/**"

  s.resources = "AtomGraphics/javascript/js/**/*.{js}"

  s.pod_target_xcconfig  = { "HEADER_SEARCH_PATHS" => "\"$(PODS_TARGET_SRCROOT)/AtomGraphics\"" }

  s.frameworks = 'JavaScriptCore','Accelerate'

  s.weak_frameworks = 'IOSurface'

end
